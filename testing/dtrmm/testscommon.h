#ifndef _TESTSCOMMON_H
#define _TESTSCOMMON_H

enum iparam_timing {
  IPARAM_RANK,       /* Rank                              */
  IPARAM_NNODES,     /* Number of nodes                   */
  IPARAM_NCORES,     /* Number of cores                   */
  IPARAM_NGPUS,      /* Number of GPUs                    */
  IPARAM_M,          /* Number of rows of the matrix      */
  IPARAM_N,          /* Number of columns of the matrix   */
  IPARAM_LDA,        /* Leading dimension of the matrix   */
  IPARAM_NRHS,       /* Number of right hand side         */
  IPARAM_LDB,        /* Leading dimension of rhs          */
  IPARAM_MB,         /* Number of rows in a tile          */
  IPARAM_NB,         /* Number of columns in a tile       */
  IPARAM_IB,         /* Inner-blocking size               */
  IPARAM_CHECK,      /* Checking activated or not         */
  IPARAM_GDROW,      /* Number of rows in the grid        */
  IPARAM_STM,        /* Number of rows in a super-tile    */
  IPARAM_STN,        /* Number of columns in a super-tile */
  IPARAM_PRIORITY,
  IPARAM_INBPARAM
};

void print_usage(void);
void runtime_init(int argc, char **argv, int *iparam);
void runtime_fini(void);
dague_context_t *setup_dague(int* pargc, char** pargv[], int *iparam, int type);
void cleanup_dague(dague_context_t* dague, char *name);

#endif /* _TESTSCOMMON_H */
