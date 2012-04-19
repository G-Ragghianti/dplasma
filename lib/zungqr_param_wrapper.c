/*
 * Copyright (c) 2011      The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 *
 * @precisions normal z -> s d c
 *
 */
#include "dague.h"
#include <plasma.h>
#include "dplasma.h"
#include "dplasma/lib/dplasmatypes.h"
#include "dplasma/lib/dplasmaaux.h"
#include "dplasma/lib/memory_pool.h"

#include "zungqr_param.h"

/***************************************************************************//**
 *
 * @ingroup dplasma_Complex64_t
 *
 *  dplasma_zungqr_param_New - Generates the dague object to generate
 *  an M-by-N matrix Q with orthonormal columns using reductions trees
 *  defined by the qrpiv structure.
 *  Q is defined as the first N columns of a product of the elementary
 *  reflectors returned by dplasma_zgeqrf.
 *  This version of zungqr is based on the hierarchical QR algorithm described
 *  in the LAWN 257.
 *
 *
 *******************************************************************************
 *
 * @param[in] qrpiv
 *          Structure generated by dplasma_pivgen_init to describe the
 *          reductions to perform during the QR factorization.
 *
 * @param[in] A
 *          Descriptor of the matrix A of size M-by-K.
 *          On entry, the i-th column must contain the vector which
 *          defines the elementary reflector H(i), for i = 1,2,...,k, as
 *          returned by dplasma_zgeqrf in the first k columns of its array
 *          argument A.
 *
 * @param[in] TS
 *          Descriptor of the auxiliary factorization data, computed
 *          by dplasma_zgeqrf_param during the TS reductions.
 *
 * @param[in] TT
 *          Descriptor of the auxiliary factorization data, computed
 *          by dplasma_zgeqrf_param during the TT reductions.
 *
 * @param[out] Q
 *          Descriptor of the M-by-N matrix Q returned.
 *
 *******************************************************************************
 *
 * @return
 *          \retval The dague object which describes the operation to perform
 *                  NULL if one of the parameter is incorrect
 *
 *******************************************************************************
 *
 * @sa dplasma_zungqr_param_Destruct
 * @sa dplasma_zungqr_param
 * @sa dplasma_cungqr_param_New
 * @sa dplasma_dorgqr_param_New
 * @sa dplasma_sorgqr_param_New
 * @sa dplasma_zgeqrf_param_New
 *
 ******************************************************************************/
dague_object_t*
dplasma_zungqr_param_New( qr_piv_t *qrpiv,
                          tiled_matrix_desc_t *A,
                          tiled_matrix_desc_t *TS,
                          tiled_matrix_desc_t *TT,
                          tiled_matrix_desc_t *Q)
{
    dague_zungqr_param_object_t* object;
    int ib = TS->mb;

    /* if ( !dplasma_check_desc(A) ) { */
    /*     dplasma_error("dplasma_zungqr_param_New", "illegal A descriptor"); */
    /*     return NULL; */
    /* } */
    /* if ( !dplasma_check_desc(T) ) { */
    /*     dplasma_error("dplasma_zungqr_param_New", "illegal T descriptor"); */
    /*     return NULL; */
    /* } */
    /* if ( !dplasma_check_desc(Q) ) { */
    /*     dplasma_error("dplasma_zungqr_param_New", "illegal Q descriptor"); */
    /*     return NULL; */
    /* } */
    if ( Q->n > Q->m ) {
        dplasma_error("dplasma_zungqr_param_New", "illegal size of Q (N should be smaller or equal to M)");
        return NULL;
    }
    if ( A->n > Q->n ) {
        dplasma_error("dplasma_zungqr_param_New", "illegal size of A (K should be smaller or equal to N)");
        return NULL;
    }
    if ( (TS->nt < A->nt) || (TS->mt < A->mt) ) {
        dplasma_error("dplasma_zungqr_param_New", "illegal size of TS (TS should have as many tiles as A)");
        return NULL;
    }
    if ( (TT->nt < A->nt) || (TT->mt < A->mt) ) {
        dplasma_error("dplasma_zungqr_param_New", "illegal size of TT (TT should have as many tiles as A)");
        return NULL;
    }

    /*
     * TODO: We consider ib is T->mb but can be incorrect for some tricks with GPU,
     * it should be passed as a parameter as in getrf
     */

    object = dague_zungqr_param_new( *A,  (dague_ddesc_t*)A,
                                     *TS, (dague_ddesc_t*)TS,
                                     *TT, (dague_ddesc_t*)TT,
                                     *Q,  (dague_ddesc_t*)Q,
                                     qrpiv, ib, NULL);

    object->p_work = (dague_memory_pool_t*)malloc(sizeof(dague_memory_pool_t));
    dague_private_memory_init( object->p_work, ib * TS->nb * sizeof(Dague_Complex64_t) );

    /* Default type */
    dplasma_add2arena_tile( object->arenas[DAGUE_zungqr_param_DEFAULT_ARENA],
                            A->mb*A->nb*sizeof(Dague_Complex64_t),
                            DAGUE_ARENA_ALIGNMENT_SSE,
                            MPI_DOUBLE_COMPLEX, A->mb );

    /* Lower triangular part of tile without diagonal */
    dplasma_add2arena_lower( object->arenas[DAGUE_zungqr_param_LOWER_TILE_ARENA],
                             A->mb*A->nb*sizeof(Dague_Complex64_t),
                             DAGUE_ARENA_ALIGNMENT_SSE,
                             MPI_DOUBLE_COMPLEX, A->mb, 0 );

    /* Little T */
    dplasma_add2arena_rectangle( object->arenas[DAGUE_zungqr_param_LITTLE_T_ARENA],
                                 TS->mb*TS->nb*sizeof(Dague_Complex64_t),
                                 DAGUE_ARENA_ALIGNMENT_SSE,
                                 MPI_DOUBLE_COMPLEX, TS->mb, TS->nb, -1);

    return (dague_object_t*)object;
}

/***************************************************************************//**
 *
 * @ingroup dplasma_Complex64_t
 *
 *  dplasma_zungqr_param_Destroy - Clean the data structures
 *  associated to a zungqr_param dague object.
 *
 *******************************************************************************
 *
 * @param[in] object
 *          Object to destroy.
 *
 *******************************************************************************
 *
 * @sa dplasma_zungqr_param_New
 * @sa dplasma_zungqr_param
 * @sa dplasma_cungqr_param_Destruct
 * @sa dplasma_dorgqr_param_Destruct
 * @sa dplasma_sorgqr_param_Destruct
 *
 ******************************************************************************/
void
dplasma_zungqr_param_Destruct( dague_object_t *o )
{
    dague_zungqr_param_object_t *dague_zungqr_param = (dague_zungqr_param_object_t *)o;

    dplasma_datatype_undefine_type( &(dague_zungqr_param->arenas[DAGUE_zungqr_param_DEFAULT_ARENA   ]->opaque_dtt) );
    dplasma_datatype_undefine_type( &(dague_zungqr_param->arenas[DAGUE_zungqr_param_LOWER_TILE_ARENA]->opaque_dtt) );
    dplasma_datatype_undefine_type( &(dague_zungqr_param->arenas[DAGUE_zungqr_param_LITTLE_T_ARENA  ]->opaque_dtt) );

    dague_private_memory_fini( dague_zungqr_param->p_work );
    free( dague_zungqr_param->p_work );

    dague_zungqr_param_destroy(dague_zungqr_param);
}


/***************************************************************************//**
 *
 * @ingroup dplasma_Complex64_t
 *
 *  dplasma_zungqr_param - Synchronous version of dplasma_zungqr_param_New
 *
 *******************************************************************************
 *
 * @param[in] dague
 *          Dague context to which submit the DAG object.
 *
 *******************************************************************************
 *
 * @return
 *          \retval 0 if success
 *          \retval < 0 if one of the parameter had an illegal value.
 *
 *******************************************************************************
 *
 * @sa dplasma_zungqr_param_Destruct
 * @sa dplasma_zungqr_param_New
 * @sa dplasma_cungqr_param
 * @sa dplasma_dorgqr_param
 * @sa dplasma_sorgqr_param
 * @sa dplasma_zgeqrf_param
 *
 ******************************************************************************/
int
dplasma_zungqr_param( dague_context_t *dague,
                      qr_piv_t *qrpiv,
                      tiled_matrix_desc_t *A,
                      tiled_matrix_desc_t *TS,
                      tiled_matrix_desc_t *TT,
                      tiled_matrix_desc_t *Q)
{
    dague_object_t *dague_zungqr_param = NULL;

    if (dague == NULL) {
        dplasma_error("dplasma_zungqr", "dplasma not initialized");
        return -1;
    }

    if ( Q->n > Q->m) {
        dplasma_error("dplasma_zungqr", "illegal number of columns in Q (N)");
        return -2;
    }
    if ( A->n > Q->n) {
        dplasma_error("dplasma_zungqr", "illegal number of columns in A (K)");
        return -3;
    }
    if ( A->m != Q->m ) {
        dplasma_error("dplasma_zungqr", "illegal number of rows in A");
        return -5;
    }

    if (imin(Q->m, imin(Q->n, A->n)) == 0)
        return 0;

    dague_zungqr_param = dplasma_zungqr_param_New(qrpiv, A, TS, TT, Q);

    dague_enqueue(dague, (dague_object_t*)dague_zungqr_param);
    dplasma_progress(dague);

    dplasma_zungqr_param_Destruct( dague_zungqr_param );
    return 0;
}
