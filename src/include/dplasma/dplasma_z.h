/*
 * Copyright (c) 2010-2022 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 *
 * @precisions normal z -> s d c
 */
#ifndef _DPLASMA_Z_H_
#define _DPLASMA_Z_H_

#include "parsec/data_dist/matrix/matrix.h"
#include "utils/dplasma_info.h"

/***********************************************************
 *               Blocking interface
 */
/* Level 2 Blas */
int dplasma_zgeadd( parsec_context_t *parsec, dplasma_enum_t trans, dplasma_complex64_t alpha,
                    const parsec_tiled_matrix_t *A, dplasma_complex64_t beta, parsec_tiled_matrix_t *B);
int dplasma_zgerc( parsec_context_t *parsec, dplasma_complex64_t alpha,
                   const parsec_tiled_matrix_t *X, const parsec_tiled_matrix_t *Y, parsec_tiled_matrix_t *A);
int dplasma_zgeru( parsec_context_t *parsec, dplasma_complex64_t alpha,
                   const parsec_tiled_matrix_t *X, const parsec_tiled_matrix_t *Y, parsec_tiled_matrix_t *A);
int dplasma_ztradd( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_complex64_t alpha,
                    const parsec_tiled_matrix_t *A, dplasma_complex64_t beta, parsec_tiled_matrix_t *B);

/* Level 3 Blas */
int dplasma_zgemm( parsec_context_t *parsec, dplasma_enum_t transA, dplasma_enum_t transB,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
int dplasma_zhemm( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t uplo,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
int dplasma_zherk( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t trans,
                   double alpha, const parsec_tiled_matrix_t *A,
                   double beta,  parsec_tiled_matrix_t *C);
int dplasma_zher2k( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t trans,
                    dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                    double beta,  parsec_tiled_matrix_t *C);
int dplasma_zsymm( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t uplo,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
int dplasma_zsyrk( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t trans,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A,
                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
int dplasma_zsyr2k( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t trans,
                    dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                    dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
int dplasma_ztrmm( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_enum_t diag,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
int dplasma_ztrsm( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_enum_t diag,
                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);

/* Level 3 Blas extensions */
int dplasma_ztrsmpl_incpiv( parsec_context_t *parsec, const parsec_tiled_matrix_t *A,
                     const parsec_tiled_matrix_t *L, const parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B);

/* Lapack */
int    dplasma_zgebrd_ge2gb(  parsec_context_t *parsec, int ib, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *Band );
int    dplasma_zgebrd_ge2gbx( parsec_context_t *parsec, int ib, dplasma_qrtree_t *qrtre0, dplasma_qrtree_t *qrtree, dplasma_qrtree_t *lqtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS0, parsec_tiled_matrix_t *TT0, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Band );
int    dplasma_zgelqf( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T);
int    dplasma_zgelqf_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT);
int    dplasma_zgelqs( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B );
int    dplasma_zgelqs_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B );
int    dplasma_zgels( parsec_context_t *parsec, dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B );
int    dplasma_zgeqrf( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T);
int    dplasma_zgeqrf_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT);
int    dplasma_zgeqrf_rec( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, int hnb);
int    dplasma_zgeqrs( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B );
int    dplasma_zgeqrs_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B );
int    dplasma_zgesv_1d ( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B );
int    dplasma_zgesv_incpiv ( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *L, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B );
int    dplasma_zgetrf_1d( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV );
int    dplasma_zgetrf_incpiv( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *L, parsec_tiled_matrix_t *IPIV );
int    dplasma_zgetrf_nopiv(  parsec_context_t *parsec, parsec_tiled_matrix_t *A );
int    dplasma_zgetrs( parsec_context_t *parsec, const dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B );
int    dplasma_zgetrs_incpiv( parsec_context_t *parsec, const dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *L, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B );
int    dplasma_zlauum( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A );
int    dplasma_zpoinv( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A);
int    dplasma_zpoinv_sync( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A);
int    dplasma_zposv ( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
int    dplasma_zpotrf( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A);
int    dplasma_zpotrf_rec( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, int hmb );
int    dplasma_zpotrs( parsec_context_t *parsec, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
int    dplasma_zpotri( parsec_context_t *parsec, dplasma_enum_t uplo, parsec_tiled_matrix_t *A);
int    dplasma_ztrtri( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_enum_t diag, parsec_tiled_matrix_t *A );
int    dplasma_zunglq( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *Q);
int    dplasma_zunglq_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Q);
int    dplasma_zungqr( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *Q);
int    dplasma_zungqr_param( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Q);
int    dplasma_zunmlq( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B);
int    dplasma_zunmlq_param( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t trans, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B );
int    dplasma_zunmqr( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B);
int    dplasma_zunmqr_param( parsec_context_t *parsec, dplasma_enum_t side, dplasma_enum_t trans, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B );

/* Lapack Auxiliary */
int    dplasma_zlacpy( parsec_context_t *parsec, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
double dplasma_zlange( parsec_context_t *parsec, dplasma_enum_t ntype, const parsec_tiled_matrix_t *A);
double dplasma_zlanhe( parsec_context_t *parsec, dplasma_enum_t ntype, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A);
double dplasma_zlanm2( parsec_context_t *parsec, const parsec_tiled_matrix_t *A, int *info);
double dplasma_zlansy( parsec_context_t *parsec, dplasma_enum_t ntype, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A);
double dplasma_zlantr( parsec_context_t *parsec, dplasma_enum_t ntype, dplasma_enum_t uplo, dplasma_enum_t diag, const parsec_tiled_matrix_t *A);
int    dplasma_zlascal(parsec_context_t *parsec, dplasma_enum_t type, dplasma_complex64_t alpha, parsec_tiled_matrix_t *A);
int    dplasma_zlaset( parsec_context_t *parsec, dplasma_enum_t uplo, dplasma_complex64_t alpha, dplasma_complex64_t beta, parsec_tiled_matrix_t *A);
int    dplasma_zlaswp( parsec_context_t *parsec, parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *IPIV, int inc);
int    dplasma_zlatms( parsec_context_t *parsec, dplasma_enum_t mtxtype, double cond, parsec_tiled_matrix_t *A, unsigned long long int seed);
int    dplasma_zplghe( parsec_context_t *parsec, double             bump, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, unsigned long long int seed);
int    dplasma_zplgsy( parsec_context_t *parsec, dplasma_complex64_t bump, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, unsigned long long int seed);
int    dplasma_zplrnt( parsec_context_t *parsec, int diagdom,                               parsec_tiled_matrix_t *A, unsigned long long int seed);

/* Auxiliary routines available only through synchronous interface */
int    dplasma_zpltmg( parsec_context_t *parsec, dplasma_enum_t mtxtype, parsec_tiled_matrix_t *A, unsigned long long int seed);
int    dplasma_zprint( parsec_context_t *parsec, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A);

/***********************************************************
 *             Non-Blocking interface
 */
/* Level 2 Blas */
parsec_taskpool_t* dplasma_zgeadd_New( dplasma_enum_t trans, dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, dplasma_complex64_t beta, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zgerc_New ( dplasma_complex64_t alpha, const parsec_tiled_matrix_t *X, const parsec_tiled_matrix_t *Y, parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_zgeru_New ( dplasma_complex64_t alpha, const parsec_tiled_matrix_t *X, const parsec_tiled_matrix_t *Y, parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_ztradd_New( dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, dplasma_complex64_t beta, parsec_tiled_matrix_t *B);

/* Level 3 Blas */
parsec_taskpool_t* dplasma_zgemm_New( dplasma_enum_t transa, dplasma_enum_t transb,
                                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zgemm_New_ex( dplasma_enum_t transA, dplasma_enum_t transB,
                                         dplasma_complex64_t alpha, const parsec_tiled_matrix_t* A, const parsec_tiled_matrix_t* B,
                                         dplasma_complex64_t beta,  parsec_tiled_matrix_t* C, dplasma_info_t opt);
parsec_taskpool_t* dplasma_zhemm_New( dplasma_enum_t side, dplasma_enum_t uplo,
                                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zherk_New( dplasma_enum_t uplo, dplasma_enum_t trans,
                                   double alpha, const parsec_tiled_matrix_t *A,
                                   double beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zher2k_New( dplasma_enum_t uplo, dplasma_enum_t trans,
                                    dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A,
                                    const parsec_tiled_matrix_t *B,
                                    double beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zsymm_New( dplasma_enum_t side, dplasma_enum_t uplo,
                                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *B,
                                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zsyrk_New( dplasma_enum_t uplo, dplasma_enum_t trans,
                                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A,
                                   dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_zsyr2k_New( dplasma_enum_t uplo, dplasma_enum_t trans,
                                    dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A,
                                    const parsec_tiled_matrix_t *B,
                                    dplasma_complex64_t beta,  parsec_tiled_matrix_t *C);
parsec_taskpool_t* dplasma_ztrmm_New( dplasma_enum_t side, dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_enum_t diag,
                                   dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_ztrsm_New( dplasma_enum_t side, dplasma_enum_t uplo, dplasma_enum_t trans, dplasma_enum_t diag,
                                   const dplasma_complex64_t alpha, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);

/* Level 3 Blas extensions */
parsec_taskpool_t* dplasma_ztrsmpl_incpiv_New(const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *L,
                                    const parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B);

/* Lapack */
parsec_taskpool_t* dplasma_zgebrd_ge2gb_New( int ib, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *Band );
parsec_taskpool_t* dplasma_zgebrd_ge2gbx_New( int ib, dplasma_qrtree_t *qrtre0, dplasma_qrtree_t *qrtree, dplasma_qrtree_t *lqtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS0, parsec_tiled_matrix_t *TT0, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Band );
parsec_taskpool_t* dplasma_zgelqf_New(parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T);
parsec_taskpool_t* dplasma_zgelqf_param_New(dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT);
parsec_taskpool_t* dplasma_zgeqrf_New(parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T);
parsec_taskpool_t* dplasma_zgeqrf_param_New(dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT);
parsec_taskpool_t* dplasma_zgetrf_1d_New(parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, int *INFO);
parsec_taskpool_t* dplasma_zgetrf_incpiv_New(parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *L, parsec_tiled_matrix_t *IPIV, int *INFO);
parsec_taskpool_t* dplasma_zgetrf_nopiv_New(parsec_tiled_matrix_t *A, int *INFO);
parsec_taskpool_t* dplasma_zlauum_New( dplasma_enum_t uplo, parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_zpoinv_New( dplasma_enum_t uplo, parsec_tiled_matrix_t *A, int *INFO);
parsec_taskpool_t* dplasma_zpotrf_New( dplasma_enum_t uplo, parsec_tiled_matrix_t *A, int *INFO);
parsec_taskpool_t* dplasma_ztrtri_New( dplasma_enum_t uplo, dplasma_enum_t diag, parsec_tiled_matrix_t *A, int *info);
parsec_taskpool_t* dplasma_zunglq_New( parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *Q);
parsec_taskpool_t* dplasma_zunglq_param_New(dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Q);
parsec_taskpool_t* dplasma_zungqr_New( parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *Q);
parsec_taskpool_t* dplasma_zungqr_param_New(dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *Q);
parsec_taskpool_t* dplasma_zunmlq_New( dplasma_enum_t side, dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zunmlq_param_New( dplasma_enum_t side, dplasma_enum_t trans, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zunmqr_New( dplasma_enum_t side, dplasma_enum_t trans, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zunmqr_param_New( dplasma_enum_t side, dplasma_enum_t trans, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, parsec_tiled_matrix_t *B);

/* Auxiliary routines */
parsec_taskpool_t* dplasma_zlacpy_New( dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zlange_New( dplasma_enum_t ntype, const parsec_tiled_matrix_t *A, double *norm);
parsec_taskpool_t* dplasma_zlanhe_New( dplasma_enum_t ntype, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, double *result);
parsec_taskpool_t* dplasma_zlanm2_New( const parsec_tiled_matrix_t *A, double *norm, int *info );
parsec_taskpool_t* dplasma_zlansy_New( dplasma_enum_t ntype, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, double *result);
parsec_taskpool_t* dplasma_zlantr_New( dplasma_enum_t ntype, dplasma_enum_t uplo, dplasma_enum_t diag, const parsec_tiled_matrix_t *A, double *result);
parsec_taskpool_t* dplasma_zlascal_New( dplasma_enum_t type, dplasma_complex64_t alpha, parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_zlaset_New( dplasma_enum_t uplo, dplasma_complex64_t alpha, dplasma_complex64_t beta, parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_zlaswp_New( parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *IPIV, int inc);
parsec_taskpool_t* dplasma_zplghe_New( double            bump, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, unsigned long long int seed);
parsec_taskpool_t* dplasma_zplgsy_New( dplasma_complex64_t bump, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, unsigned long long int seed);
parsec_taskpool_t* dplasma_zplrnt_New( int diagdom,                              parsec_tiled_matrix_t *A, unsigned long long int seed);

/* Sub-kernels to recursive DAGs */
parsec_taskpool_t* dplasma_zgeqrfr_geqrt_New(parsec_tiled_matrix_t *A,  parsec_tiled_matrix_t *T,  void *work);
parsec_taskpool_t* dplasma_zgeqrfr_tsmqr_New(parsec_tiled_matrix_t *A1, parsec_tiled_matrix_t *A2, parsec_tiled_matrix_t *V, parsec_tiled_matrix_t *T, void *work);
parsec_taskpool_t* dplasma_zgeqrfr_tsqrt_New(parsec_tiled_matrix_t *A1, parsec_tiled_matrix_t *A2, parsec_tiled_matrix_t *T, void *tau, void *work);
parsec_taskpool_t* dplasma_zgeqrfr_unmqr_New(parsec_tiled_matrix_t *A,  parsec_tiled_matrix_t *T,  parsec_tiled_matrix_t *B, void *work);

/***********************************************************
 *               Destruct functions
 */
/* Level 2 Blas */
void dplasma_zgeadd_Destruct( parsec_taskpool_t *o );
void dplasma_zgerc_Destruct( parsec_taskpool_t *o );
void dplasma_zgeru_Destruct( parsec_taskpool_t *o );
void dplasma_ztradd_Destruct( parsec_taskpool_t *o );

/* Level 3 Blas */
void dplasma_zgemm_Destruct( parsec_taskpool_t *o );
void dplasma_zhemm_Destruct( parsec_taskpool_t *o );
void dplasma_zher2k_Destruct(parsec_taskpool_t *o );
void dplasma_zherk_Destruct( parsec_taskpool_t *o );
void dplasma_zsymm_Destruct( parsec_taskpool_t *o );
void dplasma_zsyr2k_Destruct(parsec_taskpool_t *o );
void dplasma_zsyrk_Destruct( parsec_taskpool_t *o );
void dplasma_ztrmm_Destruct( parsec_taskpool_t *o );
void dplasma_ztrsm_Destruct( parsec_taskpool_t *o );

/* Level 3 Blas extensions */
void dplasma_ztrdsm_Destruct( parsec_taskpool_t *o );
void dplasma_ztrsmpl_incpiv_Destruct( parsec_taskpool_t *o );

/* Lapack */
void dplasma_zgebrd_ge2gb_Destruct( parsec_taskpool_t *o );
void dplasma_zgebrd_ge2gbx_Destruct( parsec_taskpool_t *o );
void dplasma_zgelqf_Destruct( parsec_taskpool_t *o );
void dplasma_zgelqf_param_Destruct( parsec_taskpool_t *o );
void dplasma_zgeqrf_Destruct( parsec_taskpool_t *o );
void dplasma_zgeqrf_param_Destruct( parsec_taskpool_t *o );
void dplasma_zgetrf_1d_Destruct( parsec_taskpool_t *o );
void dplasma_zgetrf_incpiv_Destruct( parsec_taskpool_t *o );
void dplasma_zgetrf_nopiv_Destruct( parsec_taskpool_t *o );
void dplasma_zlauum_Destruct( parsec_taskpool_t *o );
void dplasma_zpoinv_Destruct( parsec_taskpool_t *o );
void dplasma_zpotrf_Destruct( parsec_taskpool_t *o );
void dplasma_ztrtri_Destruct( parsec_taskpool_t *o );
void dplasma_zunglq_Destruct( parsec_taskpool_t *o );
void dplasma_zunglq_param_Destruct( parsec_taskpool_t *o );
void dplasma_zungqr_Destruct( parsec_taskpool_t *o );
void dplasma_zungqr_param_Destruct( parsec_taskpool_t *o );
void dplasma_zunmlq_Destruct( parsec_taskpool_t *o );
void dplasma_zunmlq_param_Destruct( parsec_taskpool_t *o );
void dplasma_zunmqr_Destruct( parsec_taskpool_t *o );
void dplasma_zunmqr_param_Destruct( parsec_taskpool_t *o );

/* Auxiliary routines */
void dplasma_zlacpy_Destruct( parsec_taskpool_t *o );
void dplasma_zlange_Destruct( parsec_taskpool_t *o );
void dplasma_zlanhe_Destruct( parsec_taskpool_t *o );
void dplasma_zlanm2_Destruct( parsec_taskpool_t *o );
void dplasma_zlansy_Destruct( parsec_taskpool_t *o );
void dplasma_zlantr_Destruct( parsec_taskpool_t *o );
void dplasma_zlascal_Destruct( parsec_taskpool_t *o );
void dplasma_zlaset_Destruct( parsec_taskpool_t *o );
void dplasma_zlaswp_Destruct( parsec_taskpool_t *o );
void dplasma_zplghe_Destruct( parsec_taskpool_t *o );
void dplasma_zplgsy_Destruct( parsec_taskpool_t *o );
void dplasma_zplrnt_Destruct( parsec_taskpool_t *o );

/* Sub-kernels to recursive DAGs */
void dplasma_zgeqrfr_geqrt_Destruct( parsec_taskpool_t *o );
void dplasma_zgeqrfr_tsmqr_Destruct( parsec_taskpool_t *o );
void dplasma_zgeqrfr_tsqrt_Destruct( parsec_taskpool_t *o );
void dplasma_zgeqrfr_unmqr_Destruct( parsec_taskpool_t *o );

/**********************************************************
 * Routines to set parameters in recursive DAGs
 */
void dplasma_zpotrf_setrecursive( parsec_taskpool_t *o, int hmb );
void dplasma_zgeqrf_setrecursive( parsec_taskpool_t *o, int hnb );

/**********************************************************
 * Check routines
 */
int check_zaxmb(  parsec_context_t *parsec, int loud, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *b, parsec_tiled_matrix_t *x );
int check_zpotrf( parsec_context_t *parsec, int loud, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *A0 );
int check_zpoinv( parsec_context_t *parsec, int loud, dplasma_enum_t uplo, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *Ainv );

/**********************************************************
 * Defines used to identify particular JDFs
 */
#define DPLASMA_ZGEMM_NN        1
#define DPLASMA_ZGEMM_NN_SUMMA  2
#define DPLASMA_ZGEMM_NN_GPU    3
#define DPLASMA_ZGEMM_NT        4
#define DPLASMA_ZGEMM_NT_SUMMA  5
#define DPLASMA_ZGEMM_NT_GPU    6
#define DPLASMA_ZGEMM_TN        7
#define DPLASMA_ZGEMM_TN_SUMMA  8
#define DPLASMA_ZGEMM_TN_GPU    9
#define DPLASMA_ZGEMM_TT       10
#define DPLASMA_ZGEMM_TT_SUMMA 11
#define DPLASMA_ZGEMM_TT_GPU   12

/**********************************************************
 * Work in progress
 */

/* Hybrid LU-QR */
int  dplasma_zgetrf_qrf(  parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, int criteria, double alpha, int *lu_tab, int *INFO);
int  dplasma_ztrsmpl_qrf( parsec_context_t *parsec, dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, int *lu_tab);

parsec_taskpool_t* dplasma_zgetrf_qrf_New( dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, int criteria, double alpha, int *lu_tab, int *INFO);
parsec_taskpool_t* dplasma_ztrsmpl_qrf_New(dplasma_qrtree_t *qrtree, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B, parsec_tiled_matrix_t *TS, parsec_tiled_matrix_t *TT, int *lu_tab);

void dplasma_zgetrf_qrf_Destruct( parsec_taskpool_t *o );
void dplasma_ztrsmpl_qrf_Destruct( parsec_taskpool_t *o );

/* LDLt butterfly */
int dplasma_zhebut( parsec_context_t *parsec, parsec_tiled_matrix_t *A, dplasma_complex64_t **U_but_ptr, int level);
int dplasma_zhetrf(parsec_context_t *parsec, parsec_tiled_matrix_t *A);
int dplasma_zhetrs( parsec_context_t *parsec, dplasma_enum_t uplo, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B, dplasma_complex64_t *U_but_vec, int level);
int dplasma_ztrdsm( parsec_context_t *parsec, const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B);

parsec_taskpool_t* dplasma_zgebmm_New( parsec_tiled_matrix_t *A, dplasma_complex64_t *U_but_vec, int it, int jt, int level, dplasma_enum_t trans, int *info);
parsec_taskpool_t* dplasma_zgebut_New( parsec_tiled_matrix_t *A, dplasma_complex64_t *U_but_vec, int it, int jt, int level, int *info);
parsec_taskpool_t* dplasma_zhebut_New( parsec_tiled_matrix_t *A, dplasma_complex64_t *U_but_vec, int it, int jt, int level, int *info);
parsec_taskpool_t* dplasma_zhetrf_New( parsec_tiled_matrix_t *A, int *info);
parsec_taskpool_t* dplasma_ztrdsm_New( const parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *B );
parsec_taskpool_t* dplasma_ztrmdm_New( parsec_tiled_matrix_t *A);

void dplasma_zgebmm_Destruct( parsec_taskpool_t *o );
void dplasma_zgebut_Destruct( parsec_taskpool_t *o );
void dplasma_zhebut_Destruct( parsec_taskpool_t *o );
void dplasma_zhetrf_Destruct( parsec_taskpool_t *o );
void dplasma_ztrmdm_Destruct( parsec_taskpool_t *o );

/* SVD */
parsec_taskpool_t* dplasma_zhbrdt_New(parsec_tiled_matrix_t *A);
parsec_taskpool_t* dplasma_zheev_New( const dplasma_enum_t jobz, const dplasma_enum_t uplo, parsec_tiled_matrix_t* A, parsec_tiled_matrix_t* W, parsec_tiled_matrix_t* Z, int* info );
parsec_taskpool_t* dplasma_zherbt_New( dplasma_enum_t uplo, int ib, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *T);

int dplasma_zheev( parsec_context_t *parsec, const dplasma_enum_t jobz, const dplasma_enum_t uplo, parsec_tiled_matrix_t* A, parsec_tiled_matrix_t* W, parsec_tiled_matrix_t* Z );

void dplasma_zhbrdt_Destruct( parsec_taskpool_t *o );
void dplasma_zheev_Destruct( parsec_taskpool_t *o );
void dplasma_zherbt_Destruct( parsec_taskpool_t *o );

/* LU with partial pivoting and full PTG panel */
int  dplasma_ztrsmpl_ptgpanel( parsec_context_t *parsec, const parsec_tiled_matrix_t *A,
                             const parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B);
int  dplasma_zgetrf_ptgpanel( parsec_context_t *parsec, parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV);
int  dplasma_zgerfs( parsec_context_t *parsec, parsec_tiled_matrix_t* ddescA, parsec_tiled_matrix_t* ddescLU, parsec_tiled_matrix_t* ddescB, parsec_tiled_matrix_t* ddescX);

parsec_taskpool_t* dplasma_ztrsmpl_ptgpanel_New(const parsec_tiled_matrix_t *A, const parsec_tiled_matrix_t *IPIV, parsec_tiled_matrix_t *B);
parsec_taskpool_t* dplasma_zgetrf_ptgpanel_New( parsec_tiled_matrix_t *A, parsec_tiled_matrix_t *IPIV, int *info );

void dplasma_ztrsmpl_ptgpanel_Destruct( parsec_taskpool_t *o );
void dplasma_zgetrf_ptgpanel_Destruct( parsec_taskpool_t *o );

#endif /* _DPLASMA_Z_H_ */
