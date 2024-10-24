/*
 * Copyright (c) 2011-2022 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2013      Inria. All rights reserved.
 * $COPYRIGHT
 *
 * @precisions normal z -> s d c
 *
 */

#include "dplasma.h"
#include "dplasma/types.h"
#include "dplasmaaux.h"
#include "parsec/private_mempool.h"

#include "zgetrf_incpiv.h"

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 * dplasma_zgetrf_incpiv_New - Generates the taskpool that computes the LU
 * factorization of a M-by-N matrix A using incremental pivoting tile algorithm.
 *
 * Note that incremental pivoting LU is not as tolerant to round-off errors
 * as standard partial pivoting LU, in particular with matrices where some
 * off-diagonal elements are much larger than diagonal elements.
 *
 * Other variants of LU decomposition are available in the library wioth the
 * following function:
 *     - dplasma_zgetrf_New() that performs LU decomposition with partial pivoting.
 *       This is limited to matrices with column-cyclic distribution.
 *     - dplasma_zgetrf_nopiv_New() that performs LU decomposition with no pivoting
 *       if the matrix is known as beeing diagonal dominant.
 *     - dplasma_zgetrf_qrf_New() that performs an hybrid LU-QR decomposition.
 *
 * WARNING: The computations are not done by this call.
 *
 *******************************************************************************
 *
 * @param[in,out] A
 *          Descriptor of the distributed matrix A to be factorized.
 *          On entry, describes the M-by-N matrix A.
 *          On exit, elements on and above the diagonal are the elements of
 *          U. Elements belowe the diagonal are NOT the classic L, but the L
 *          factors obtaines by succesive pivoting.
 *
 * @param[out] L
 *          Descriptor of the matrix L distributed exactly as the A matrix.
 *          L.mb defines the IB parameter of the tile LU
 *          algorithm. This matrix must be of size A.mt * L.mb - by - A.nt *
 *          L.nb, with L.nb == A.nb.
 *          On exit, contains auxiliary information required to solve the system.
 *
 * @param[out] IPIV
 *          Descriptor of the IPIV matrix. Should be distributed exactly as the
 *          A matrix. This matrix must be of size A.m - by - A.nt with IPIV.mb =
 *          A.mb and IPIV.nb = 1.
 *          On exit, contains the pivot indices of the successive row
 *          interchanged performed during the factorization.
 *
 * @param[out] INFO
 *          On algorithm completion: equal to 0 on success, i if the ith
 *          diagonal value is equal to 0. That implies incoherent result.
 *
 *******************************************************************************
 *
 * @return
 *          \retval NULL if incorrect parameters are given.
 *          \retval The parsec taskpool describing the operation that can be
 *          enqueued in the runtime with parsec_context_add_taskpool(). It, then, needs to be
 *          destroy with dplasma_zgetrf_incpiv_Destruct();
 *
 *******************************************************************************
 *
 * @sa dplasma_zgetrf_incpiv
 * @sa dplasma_zgetrf_incpiv_Destruct
 * @sa dplasma_cgetrf_incpiv_New
 * @sa dplasma_dgetrf_incpiv_New
 * @sa dplasma_sgetrf_incpiv_New
 *
 ******************************************************************************/
parsec_taskpool_t*
dplasma_zgetrf_incpiv_New( parsec_tiled_matrix_t *A,
                           parsec_tiled_matrix_t *L,
                           parsec_tiled_matrix_t *IPIV,
                           int *INFO )
{
    parsec_zgetrf_incpiv_taskpool_t *parsec_getrf_incpiv;
    int ib;

    if ( (A->mt != L->mt) || (A->nt != L->nt) ) {
        dplasma_error("dplasma_zgetrf_incpiv_New", "L doesn't have the same number of tiles as A");
        return NULL;
    }
    if ( (A->mt != IPIV->mt) || (A->nt != IPIV->nt) ) {
        dplasma_error("dplasma_zgetrf_incpiv_New", "IPIV doesn't have the same number of tiles as A");
        return NULL;
    }

    ib = L->mb;
    parsec_getrf_incpiv = parsec_zgetrf_incpiv_new( A,
                                                    L,
                                                    (parsec_data_collection_t*)IPIV,
                                                    INFO, NULL);

    parsec_getrf_incpiv->_g_work_pool = (parsec_memory_pool_t*)malloc(sizeof(parsec_memory_pool_t));
    parsec_private_memory_init( parsec_getrf_incpiv->_g_work_pool, ib * L->nb * sizeof(dplasma_complex64_t) );

    /* A */
    dplasma_add2arena_tile( &parsec_getrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_DEFAULT_ADT_IDX],
                            A->mb*A->nb*sizeof(dplasma_complex64_t),
                            PARSEC_ARENA_ALIGNMENT_SSE,
                            parsec_datatype_double_complex_t, A->mb );

    /* Lower part of A without diagonal part */
    dplasma_add2arena_lower( &parsec_getrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_LOWER_TILE_ADT_IDX],
                             A->mb*A->nb*sizeof(dplasma_complex64_t),
                             PARSEC_ARENA_ALIGNMENT_SSE,
                             parsec_datatype_double_complex_t, A->mb, 0 );

    /* Upper part of A with diagonal part */
    dplasma_add2arena_upper( &parsec_getrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_UPPER_TILE_ADT_IDX],
                             A->mb*A->nb*sizeof(dplasma_complex64_t),
                             PARSEC_ARENA_ALIGNMENT_SSE,
                             parsec_datatype_double_complex_t, A->mb, 1 );

    /* IPIV */
    dplasma_add2arena_rectangle( &parsec_getrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_PIVOT_ADT_IDX],
                                 A->mb*sizeof(int),
                                 PARSEC_ARENA_ALIGNMENT_SSE,
                                 parsec_datatype_int_t, A->mb, 1, -1 );

    /* L */
    dplasma_add2arena_rectangle( &parsec_getrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_SMALL_L_ADT_IDX],
                                 L->mb*L->nb*sizeof(dplasma_complex64_t),
                                 PARSEC_ARENA_ALIGNMENT_SSE,
                                 parsec_datatype_double_complex_t, L->mb, L->nb, -1);


    return (parsec_taskpool_t*)parsec_getrf_incpiv;
}

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 *  dplasma_zgetrf_incpiv_Destruct - Free the data structure associated to an taskpool
 *  created with dplasma_zgetrf_incpiv_New().
 *
 *******************************************************************************
 *
 * @param[in,out] taskpool
 *          On entry, the taskpool to destroy.
 *          On exit, the taskpool cannot be used anymore.
 *
 *******************************************************************************
 *
 * @sa dplasma_zgetrf_incpiv_New
 * @sa dplasma_zgetrf_incpiv
 *
 ******************************************************************************/
void
dplasma_zgetrf_incpiv_Destruct( parsec_taskpool_t *tp )
{
    parsec_zgetrf_incpiv_taskpool_t *parsec_zgetrf_incpiv = (parsec_zgetrf_incpiv_taskpool_t *)tp;

    dplasma_matrix_del2arena( &parsec_zgetrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_DEFAULT_ADT_IDX   ] );
    dplasma_matrix_del2arena( &parsec_zgetrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_UPPER_TILE_ADT_IDX] );
    dplasma_matrix_del2arena( &parsec_zgetrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_LOWER_TILE_ADT_IDX] );
    dplasma_matrix_del2arena( &parsec_zgetrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_SMALL_L_ADT_IDX   ] );
    dplasma_matrix_del2arena( &parsec_zgetrf_incpiv->arenas_datatypes[PARSEC_zgetrf_incpiv_PIVOT_ADT_IDX     ] );

    parsec_private_memory_fini( parsec_zgetrf_incpiv->_g_work_pool );
    free( parsec_zgetrf_incpiv->_g_work_pool );

    parsec_taskpool_free(tp);
}

/**
 *******************************************************************************
 *
 * @ingroup dplasma_complex64
 *
 * dplasma_zgetrf_incpiv_New - Generates the taskpool that computes the LU
 * factorization of a M-by-N matrix A using incremental pivoting tile algorithm.
 *
 * Note that incremental pivoting LU is not as tolerant to round-off errors
 * as standard partial pivoting LU, in particular with matrices where some
 * off-diagonal elements are much larger than diagonal elements.
 *
 * Other variants of LU decomposition are available in the library wioth the
 * following function:
 *     - dplasma_zgetrf_New() that performs LU decomposition with partial pivoting.
 *       This is limited to matrices with column-cyclic distribution.
 *     - dplasma_zgetrf_nopiv_New() that performs LU decomposition with no pivoting
 *       if the matrix is known as beeing diagonal dominant.
 *     - dplasma_zgetrf_qrf_New() that performs an hybrid LU-QR decomposition.
 *
 *******************************************************************************
 *
 * @param[in,out] parsec
 *          The parsec context of the application that will run the operation.
 *
 * @param[in,out] A
 *          Descriptor of the distributed matrix A to be factorized.
 *          On entry, describes the M-by-N matrix A.
 *          On exit, elements on and above the diagonal are the elements of
 *          U. Elements belowe the diagonal are NOT the classic L, but the L
 *          factors obtaines by succesive pivoting.
 *
 * @param[out] L
 *          Descriptor of the matrix L distributed exactly as the A matrix.
 *          L.mb defines the IB parameter of the tile LU
 *          algorithm. This matrix must be of size A.mt * L.mb - by - A.nt *
 *          L.nb, with L.nb == A.nb.
 *          On exit, contains auxiliary information required to solve the system.
 *
 * @param[out] IPIV
 *          Descriptor of the IPIV matrix. Should be distributed exactly as the
 *          A matrix. This matrix must be of size A.m - by - A.nt with IPIV.mb =
 *          A.mb and IPIV.nb = 1.
 *          On exit, contains the pivot indices of the successive row
 *          interchanged performed during the factorization.
 *
 *******************************************************************************
 *
 * @return
 *          \retval -i if the ith parameters is incorrect.
 *          \retval 0 on success.
 *          \retval i if ith value is singular. Result is incoherent.
 *
 *******************************************************************************
 *
 * @sa dplasma_zgetrf_incpiv
 * @sa dplasma_zgetrf_incpiv_Destruct
 * @sa dplasma_cgetrf_incpiv_New
 * @sa dplasma_dgetrf_incpiv_New
 * @sa dplasma_sgetrf_incpiv_New
 *
 ******************************************************************************/
int
dplasma_zgetrf_incpiv( parsec_context_t *parsec,
                       parsec_tiled_matrix_t *A,
                       parsec_tiled_matrix_t *L,
                       parsec_tiled_matrix_t *IPIV )
{
    parsec_taskpool_t *parsec_zgetrf_incpiv = NULL;
    int info = 0;

    if ( (A->mt != L->mt) || (A->nt != L->nt) ) {
        dplasma_error("dplasma_zgetrf_incpiv", "L doesn't have the same number of tiles as A");
        return -3;
    }
    if ( (A->mt != IPIV->mt) || (A->nt != IPIV->nt) ) {
        dplasma_error("dplasma_zgetrf_incpiv", "IPIV doesn't have the same number of tiles as A");
        return -4;
    }

    parsec_zgetrf_incpiv = dplasma_zgetrf_incpiv_New(A, L, IPIV, &info);

    if ( parsec_zgetrf_incpiv != NULL ) {
        parsec_context_add_taskpool( parsec, parsec_zgetrf_incpiv );
        dplasma_wait_until_completion(parsec);
        dplasma_zgetrf_incpiv_Destruct( parsec_zgetrf_incpiv );
        return info;
    }
    else
        return -101;
}
