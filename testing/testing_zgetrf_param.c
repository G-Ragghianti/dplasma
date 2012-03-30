/*
 * Copyright (c) 2009-2011 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 *
 * @precisions normal z -> s d c
 *
 */

#include "common.h"
#include "data_dist/matrix/two_dim_rectangle_cyclic.h"

static int check_inverse( dague_context_t *dague, int loud,
                          tiled_matrix_desc_t *ddescA,
                          tiled_matrix_desc_t *ddescInvA,
                          tiled_matrix_desc_t *ddescI );

static int check_solution( dague_context_t *dague, int loud,
                           tiled_matrix_desc_t *ddescA,
                           tiled_matrix_desc_t *ddescB,
                           tiled_matrix_desc_t *ddescX);

int main(int argc, char ** argv)
{
    dague_context_t* dague;
    int iparam[IPARAM_SIZEOF];
    int info_sol = 0;
    int info = 0, ret = 0;
    qr_piv_t *qrpiv;

    /* Set defaults for non argv iparams */
    iparam_default_facto(iparam);
    iparam_default_ibnbmb(iparam, 48, 144, 144);
    iparam[IPARAM_LDA] = -'m';
    iparam[IPARAM_LDB] = -'m';

    /* Initialize DAGuE */
    dague = setup_dague(argc, argv, iparam);
    PASTE_CODE_IPARAM_LOCALS(iparam)
    PASTE_CODE_FLOPS(FLOPS_ZGETRF, ((DagDouble_t)M,(DagDouble_t)N))

    LDA = max(M, LDA);

	if ( M != N && check ) {
        fprintf(stderr, "Check is impossible if M != N\n");
        check = 0;
	}

    /* initializing matrix structure */
    PASTE_CODE_ALLOCATE_MATRIX(ddescA, 1,
        two_dim_block_cyclic, (&ddescA, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDA, N, 0, 0,
                               M, N, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescLT, 1,
        two_dim_block_cyclic, (&ddescLT, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, IB, NB, MT*IB, N, 0, 0,
                               MT*IB, N, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescIPIV, 1,
        two_dim_block_cyclic, (&ddescIPIV, matrix_Integer, matrix_Tile,
                               nodes, cores, rank, MB, 1, M, NT, 0, 0,
                               M, NT, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescA0, check,
        two_dim_block_cyclic, (&ddescA0, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDA, N, 0, 0,
                               M, N, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescB, check,
        two_dim_block_cyclic, (&ddescB, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDB, NRHS, 0, 0,
                               M, NRHS, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescX, check,
        two_dim_block_cyclic, (&ddescX, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDB, NRHS, 0, 0,
                               M, NRHS, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescInvA, check,
        two_dim_block_cyclic, (&ddescInvA, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDA, N, 0, 0,
                               M, N, SMB, SNB, P));
    PASTE_CODE_ALLOCATE_MATRIX(ddescI, check,
        two_dim_block_cyclic, (&ddescI, matrix_ComplexDouble, matrix_Tile,
                               nodes, cores, rank, MB, NB, LDA, N, 0, 0,
                               M, N, SMB, SNB, P));

    /* matrix generation */
    if(loud > 2) printf("+++ Generate matrices ... ");
    dplasma_zplrnt( dague, (tiled_matrix_desc_t *)&ddescA, 3872);
    dplasma_zlaset( dague, PlasmaUpperLower, 0., 0., (tiled_matrix_desc_t *)&ddescLT);
    qrpiv = dplasma_pivgen_init( (tiled_matrix_desc_t *)&ddescA,
                                 iparam[IPARAM_LOWLVL_TREE], 
                                 iparam[IPARAM_HIGHLVL_TREE],
                                 iparam[IPARAM_QR_TS_SZE],
                                 iparam[IPARAM_QR_HLVL_SZE],
                                 iparam[IPARAM_QR_DOMINO],
                                 iparam[IPARAM_QR_TSRR] );
	if ( check ) {
        dplasma_zlacpy( dague, PlasmaUpperLower,
                        (tiled_matrix_desc_t *)&ddescA,
                        (tiled_matrix_desc_t *)&ddescA0 );
		dplasma_zplrnt( dague, (tiled_matrix_desc_t *)&ddescB, 9867 );
        dplasma_zlacpy( dague, PlasmaUpperLower,
                        (tiled_matrix_desc_t *)&ddescB,
                        (tiled_matrix_desc_t *)&ddescX );
		dplasma_zlaset( dague, PlasmaUpperLower, 0., 1., (tiled_matrix_desc_t *)&ddescI);
		dplasma_zlaset( dague, PlasmaUpperLower, 0., 1., (tiled_matrix_desc_t *)&ddescInvA);
    }
    if(loud > 2) printf("Done\n");

    /* Create DAGuE */
    if(loud > 2) printf("+++ Computing getrf_param ... ");
    PASTE_CODE_ENQUEUE_KERNEL(dague, zgetrf_param,
                              (qrpiv,
                               (tiled_matrix_desc_t*)&ddescA,
                               (tiled_matrix_desc_t*)&ddescIPIV,
                               (tiled_matrix_desc_t*)&ddescLT,
                               &info));

    /* lets rock! */
    printf("\nLet's rock the factorisation!\n");
    PASTE_CODE_PROGRESS_KERNEL(dague, zgetrf_param);
    dplasma_zgetrf_param_Destruct( DAGUE_zgetrf_param );
    if(loud > 2) printf("Done.\n");
    printf("\nFactorisation is done!\n");

    if ( info != 0 ) {
        if( rank == 0 && loud ) printf("-- Factorization is suspicious (info = %d) ! \n", info );
        ret |= 1;
    }
    else if( check ) {
        /*
         * First check with a right hand side
         */
		printf("\nLet's apply modifications to B for right hand side\n");
        dplasma_ztrsmpl_param( dague, qrpiv,
                              (tiled_matrix_desc_t *)&ddescA,
                              (tiled_matrix_desc_t *)&ddescX,
                              (tiled_matrix_desc_t *)&ddescIPIV,
                              (tiled_matrix_desc_t *)&ddescLT,
                              &info);
		printf("\nB for right hand side is done!\n");
		printf("\nLet's solve the triangular system for right hand side!\n");
        dplasma_ztrsm(dague, PlasmaLeft, PlasmaUpper, PlasmaNoTrans, PlasmaNonUnit, 1.0,
                      (tiled_matrix_desc_t *)&ddescA,
                      (tiled_matrix_desc_t *)&ddescX);
		printf("\nTriangular system for right hand side solved!\n");

        /* Check the solution */
        ret |= check_solution(dague, (rank == 0) ? loud : 0,
                              (tiled_matrix_desc_t *)&ddescA0,
                              (tiled_matrix_desc_t *)&ddescB,
                              (tiled_matrix_desc_t *)&ddescX);

        /*
         * First check with inverse
         */
		printf("\nLet's apply modifications to B for inverse\n");
        dplasma_ztrsmpl_param( dague, qrpiv,
                              (tiled_matrix_desc_t *)&ddescA,
                              (tiled_matrix_desc_t *)&ddescInvA,
                              (tiled_matrix_desc_t *)&ddescIPIV,
                              (tiled_matrix_desc_t *)&ddescLT,
                              &info);
		printf("\nB for inverse is done!\n");
		printf("\nLet's solve the triangular system for inverse!\n");
        dplasma_ztrsm(dague, PlasmaLeft, PlasmaUpper, PlasmaNoTrans, PlasmaNonUnit, 1.0,
                      (tiled_matrix_desc_t *)&ddescA,
                      (tiled_matrix_desc_t *)&ddescInvA);
		printf("\nTriangular system solved for inverse!\n");

        /* Check the solution */
        ret |= check_inverse(dague, (rank == 0) ? loud : 0,
                             (tiled_matrix_desc_t *)&ddescA0,
                             (tiled_matrix_desc_t *)&ddescInvA,
                             (tiled_matrix_desc_t *)&ddescI);
    }

    dplasma_pivgen_finalize( qrpiv );

    cleanup_dague(dague, iparam);

    dague_data_free(ddescA.mat);
    dague_data_free(ddescLT.mat);
    dague_data_free(ddescIPIV.mat);
    
    dague_ddesc_destroy((dague_ddesc_t*)&ddescA);
    dague_ddesc_destroy((dague_ddesc_t*)&ddescLT);
    dague_ddesc_destroy((dague_ddesc_t*)&ddescIPIV);
    
    if ( check ) {
        dague_data_free(ddescA0.mat);
        dague_data_free(ddescInvA.mat);
        dague_data_free(ddescI.mat);
        dague_data_free(ddescB.mat);
        dague_data_free(ddescX.mat);
    
        dague_ddesc_destroy((dague_ddesc_t*)&ddescA0);
        dague_ddesc_destroy((dague_ddesc_t*)&ddescInvA);
        dague_ddesc_destroy((dague_ddesc_t*)&ddescB);
        dague_ddesc_destroy((dague_ddesc_t*)&ddescX);
        dague_ddesc_destroy((dague_ddesc_t*)&ddescI);
    } 
    return info_sol;
}


static int check_solution( dague_context_t *dague, int loud,
                           tiled_matrix_desc_t *ddescA,
                           tiled_matrix_desc_t *ddescB,
                           tiled_matrix_desc_t *ddescX )
{
    int info_solution;
    double Rnorm = 0.0;
    double Anorm = 0.0;
    double Bnorm = 0.0;
    double Xnorm, result;
    int m = ddescB->m;
    double eps = LAPACKE_dlamch_work('e');

    Anorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescA);
    Bnorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescB);
    Xnorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescX);

    /* Compute b - A*x */
    dplasma_zgemm( dague, PlasmaNoTrans, PlasmaNoTrans, -1.0, ddescA, ddescX, 1.0, ddescB);

    Rnorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescB);

    result = Rnorm / ( ( Anorm * Xnorm + Bnorm ) * m * eps ) ;

    if ( loud > 2 ) {
        printf("============\n");
        printf("Checking the Residual of the solution \n");
        if ( loud > 3 )
            printf( "-- ||A||_oo = %e, ||X||_oo = %e, ||B||_oo= %e, ||A X - B||_oo = %e\n",
                    Anorm, Xnorm, Bnorm, Rnorm );

        printf("-- ||Ax-B||_oo/((||A||_oo||x||_oo+||B||_oo).N.eps) = %e \n", result);
    }

    if (  isnan(Xnorm) || isinf(Xnorm) || isnan(result) || isinf(result) || (result > 60.0) ) {
        if( loud ) printf("-- Solution is suspicious ! \n");
        info_solution = 1;
    }
    else{
        if( loud ) printf("-- Solution is CORRECT ! \n");
        info_solution = 0;
    }

    return info_solution;
}

static int check_inverse( dague_context_t *dague, int loud,
                          tiled_matrix_desc_t *ddescA,
                          tiled_matrix_desc_t *ddescInvA,
                          tiled_matrix_desc_t *ddescI )
{
    int info_solution;
    double Anorm    = 0.0;
    double InvAnorm = 0.0;
    double Rnorm, result;
    int m = ddescA->m;
    double eps = LAPACKE_dlamch_work('e');

    Anorm    = dplasma_zlange(dague, PlasmaMaxNorm, ddescA   );
    InvAnorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescInvA);

    /* Compute I - A*A-1 */
    dplasma_zgemm( dague, PlasmaNoTrans, PlasmaNoTrans, -1.0, ddescA, ddescInvA, 1.0, ddescI);

    Rnorm = dplasma_zlange(dague, PlasmaMaxNorm, ddescI);

    result = Rnorm / ( ( Anorm * InvAnorm ) * m * eps ) ;

    if ( loud > 2 ) {
        printf("============\n");
        printf("Checking the Residual of the solution \n");
        if ( loud > 3 )
            printf( "-- ||A||_oo = %e, ||A-1||_oo = %e, ||A A-1 - I||_oo = %e\n",
                    Anorm, InvAnorm, Rnorm );

        printf("-- ||AA^-1-I||_oo/((||A||_oo||A^-1||_oo).N.eps) = %e \n", result);
    }

    if (  isnan(Rnorm) || isinf(Rnorm) || isnan(result) || isinf(result) || (result > 60.0) ) {
        if( loud ) printf("-- Solution is suspicious ! \n");
        info_solution = 1;
    }
    else{
        if( loud ) printf("-- Solution is CORRECT ! \n");
        info_solution = 0;
    }

    return info_solution;
}
	
