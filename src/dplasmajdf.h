/*
 * Copyright (c) 2020-2024 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 */
#ifndef _DPLASMAJDF_H_
#define _DPLASMAJDF_H_

#include "dplasma.h"
#include "dplasmaaux.h"
#include "cores/core_blas.h"
#include "cores/dplasma_cores.h"
#include "parsec/private_mempool.h"
#include "floputils.h"

/* sqrt function; these names needed for the precision generator stage */
#define dplasma_zsqrt csqrt
#define dplasma_csqrt csqrtf
#define dplasma_dsqrt sqrt
#define dplasma_ssqrt sqrtf

#define QUOTEME_(x) #x
#define QUOTEME(x) QUOTEME_(x)


#ifdef DPLASMA_TRACE_KERNELS
#   include <stdlib.h>
#   include <stdio.h>
#   define printlog(str, ...) fprintf(stderr, "thread %d VP %d " str "\n", \
                                      es->th_id, es->virtual_process->vp_id, __VA_ARGS__)
#   define printloggpu(str, ...) fprintf(stderr, "GPU %s " str "\n", \
                                          gpu_device->super.device_name, __VA_ARGS__)
#else
#   define printlog(...) do {} while(0)
#   define printloggpu(...) do {} while(0)
#endif

#ifndef PARSEC_HAVE_MPI
#define TEMP_TYPE MPITYPE
#undef MPITYPE
#define MPITYPE ((parsec_datatype_t)QUOTEME(TEMP_TYPE))
#undef TEMP_TYPE
#endif  /* PARSEC_HAVE_MPI */

#endif /* _DPLASMAJDF_H_ */

