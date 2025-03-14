/*
 * Copyright (c) 2010-2022 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 *
 * @precisions normal z -> z c
 *
 */

#include "dplasma.h"
#include "dplasma/types.h"
#include "dplasmaaux.h"
#include "cores/core_blas.h"

#include "zherk_LN.h"
#include "zherk_LC.h"
#include "zherk_UN.h"
#include "zherk_UC.h"

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 *  dplasm_zherk_New - Generates the taskpool that performs the following operation
 *
 *    \f[ C = \alpha [ op( A ) \times conjg( op( A )' )] + \beta C \f],
 *
 *  where op( X ) is one of
 *
 *    op( X ) = X  or op( X ) = conjg( X' )
 *
 *  where alpha and beta are real scalars, C is an n-by-n hermitian
 *  matrix and A is an n-by-k matrix in the first case and a k-by-n
 *  matrix in the second case.
 *
 *  WARNING: The computations are not done by this call.
 *
 *******************************************************************************
 *
 * @param[in] uplo
 *          = dplasmaUpper: Upper triangle of C is stored;
 *          = dplasmaLower: Lower triangle of C is stored.
 *
 * @param[in] trans
 *          Specifies whether the matrix A is transposed or conjugate transposed:
 *          = dplasmaNoTrans:   A is not transposed;
 *          = dplasmaConjTrans: A is conjugate transposed.
 *
 * @param[in] alpha
 *          alpha specifies the scalar alpha.
 *
 * @param[in] A
 *          A is a LDA-by-ka matrix, where ka is K when trans = dplasmaNoTrans,
 *          and is N otherwise.
 *
 * @param[in] beta
 *          beta specifies the scalar beta
 *
 * @param[in,out] C
 *          C is a LDC-by-N matrix.
 *          On exit, the array uplo part of the matrix is overwritten
 *          by the uplo part of the updated matrix.
 *
 *******************************************************************************
 *
 * @return
 *          \retval NULL if incorrect parameters are given.
 *          \retval The parsec taskpool describing the operation that can be
 *          enqueued in the runtime with parsec_context_add_taskpool(). It, then, needs to be
 *          destroy with dplasma_zherk_Destruct();
 *
 *******************************************************************************
 *
 * @sa dplasma_zherk
 * @sa dplasma_zherk_Destruct
 * @sa dplasma_cherk_New
 *
 ******************************************************************************/
parsec_taskpool_t*
dplasma_zherk_New( dplasma_enum_t uplo,
                   dplasma_enum_t trans,
                   double alpha,
                   const parsec_tiled_matrix_t* A,
                   double beta,
                   parsec_tiled_matrix_t* C)
{
    parsec_taskpool_t* tp;

    if ( uplo == dplasmaLower ) {
        if ( trans == dplasmaNoTrans ) {
            tp = (parsec_taskpool_t*)
                parsec_zherk_LN_new(uplo, trans,
                                   alpha, A,
                                   beta,  C);
        }
        else {
            tp = (parsec_taskpool_t*)
                parsec_zherk_LC_new(uplo, trans,
                                   alpha, A,
                                   beta,  C);
        }
    }
    else {
        if ( trans == dplasmaNoTrans ) {
            tp = (parsec_taskpool_t*)
                parsec_zherk_UN_new(uplo, trans,
                                   alpha, A,
                                   beta,  C);
        }
        else {
            tp = (parsec_taskpool_t*)
                parsec_zherk_UC_new(uplo, trans,
                                   alpha, A,
                                   beta,  C);
        }
    }

    dplasma_add2arena_tile( &((parsec_zherk_LN_taskpool_t*)tp)->arenas_datatypes[PARSEC_zherk_LN_DEFAULT_ADT_IDX],
                            C->mb*C->nb*sizeof(dplasma_complex64_t),
                            PARSEC_ARENA_ALIGNMENT_SSE,
                            parsec_datatype_double_complex_t, C->mb );

    return tp;
}

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 *  dplasma_zherk_Destruct - Free the data structure associated to an taskpool
 *  created with dplasma_zherk_New().
 *
 *******************************************************************************
 *
 * @param[in,out] taskpool
 *          On entry, the taskpool to destroy.
 *          On exit, the taskpool cannot be used anymore.
 *
 *******************************************************************************
 *
 * @sa dplasma_zherk_New
 * @sa dplasma_zherk
 *
 ******************************************************************************/
void
dplasma_zherk_Destruct( parsec_taskpool_t *tp )
{
    parsec_zherk_LN_taskpool_t *zherk_tp = (parsec_zherk_LN_taskpool_t*)tp;
    dplasma_matrix_del2arena( &zherk_tp->arenas_datatypes[PARSEC_zherk_LN_DEFAULT_ADT_IDX] );
    parsec_taskpool_free(tp);
}

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 *  dplasm_zherk - Performs the following operation
 *
 *    \f[ C = \alpha [ op( A ) \times conjg( op( A )' )] + \beta C \f],
 *
 *  where op( X ) is one of
 *
 *    op( X ) = X  or op( X ) = conjg( X' )
 *
 *  where alpha and beta are real scalars, C is an n-by-n hermitian
 *  matrix and A is an n-by-k matrix in the first case and a k-by-n
 *  matrix in the second case.
 *
 *******************************************************************************
 *
 * @param[in,out] parsec
 *          The parsec context of the application that will run the operation.
 *
 * @param[in] uplo
 *          = dplasmaUpper: Upper triangle of C is stored;
 *          = dplasmaLower: Lower triangle of C is stored.
 *
 * @param[in] trans
 *          Specifies whether the matrix A is transposed or conjugate transposed:
 *          = dplasmaNoTrans:   A is not transposed;
 *          = dplasmaConjTrans: A is conjugate transposed.
 *
 * @param[in] alpha
 *          alpha specifies the scalar alpha.
 *
 * @param[in] A
 *          A is a LDA-by-ka matrix, where ka is K when trans = dplasmaNoTrans,
 *          and is N otherwise.
 *
 * @param[in] beta
 *          beta specifies the scalar beta
 *
 * @param[in,out] C
 *          C is a LDC-by-N matrix.
 *          On exit, the array uplo part of the matrix is overwritten
 *          by the uplo part of the updated matrix.
 *
 *******************************************************************************
 *
 * @return
 *          \retval -i if the ith parameters is incorrect.
 *          \retval 0 on success.
 *
 *******************************************************************************
 *
 * @sa dplasma_zherk_New
 * @sa dplasma_zherk_Destruct
 * @sa dplasma_cherk
 *
 ******************************************************************************/
int
dplasma_zherk( parsec_context_t *parsec,
               dplasma_enum_t uplo,
               dplasma_enum_t trans,
               double alpha,
               const parsec_tiled_matrix_t *A,
               double beta,
               parsec_tiled_matrix_t *C)
{
    parsec_taskpool_t *parsec_zherk = NULL;

    /* Check input arguments */
    if ((uplo != dplasmaLower) && (uplo != dplasmaUpper)) {
        dplasma_error("PLASMA_zherk", "illegal value of uplo");
        return -1;
    }
    if (trans != dplasmaConjTrans && trans != dplasmaNoTrans ) {
        dplasma_error("dplasma_zherk", "illegal value of trans");
        return -2;
    }
    if ( (C->m != C->n) ) {
        dplasma_error("dplasma_zherk", "illegal size of matrix C which should be square");
        return -6;
    }
    if ( ((trans == dplasmaNoTrans) && (A->m != C->m)) ||
         ((trans != dplasmaNoTrans) && (A->n != C->m)) ) {
        dplasma_error("dplasma_zherk", "illegal size of matrix A");
        return -4;
    }

    parsec_zherk = dplasma_zherk_New(uplo, trans,
                                    alpha, A,
                                    beta, C);

    if ( parsec_zherk != NULL )
    {
        parsec_context_add_taskpool( parsec, parsec_zherk);
        dplasma_wait_until_completion(parsec);
        dplasma_zherk_Destruct( parsec_zherk );
    }
    return 0;
}
