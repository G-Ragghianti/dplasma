#
# Copyright (c) 2011-2022 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
#
set(ZHEADERS
  dplasma_zcores.h core_zblas.h
)
set(ZSOURCES
  core_dzasum.c
  core_zamax.c
  core_zgeadd.c
  core_zgelqt.c
  core_zgemdm.c
  core_zgemm.c
  core_zgemv.c
  core_zgeqrt.c
  core_zgessm.c
  core_zgessq.c
  core_zgetf2_nopiv.c
  core_zgetrf_incpiv.c
  core_zgetrf_nopiv.c
  core_zgetrf_reclap.c
  core_zgetrf_rectil.c
  core_zhebut.c
  core_zhedrk.c
  core_zhemm.c
  core_zher2k.c
  core_zherfb.c
  core_zherk.c
  core_zhessq.c
  core_zhetrf2_nopiv.c
  core_zhetrf_nopiv.c
  core_zlacpy.c
  core_zlange.c
  core_zlanhe.c
  core_zlansy.c
  core_zlantr.c
  core_zlaset.c
  core_zlaswp.c
  core_zlatro.c
  core_zlauum.c
  core_zpamm.c
  core_zparfb.c
  core_zpemv.c
  core_zplghe.c
  core_zplgsy.c
  core_zplrnt.c
  core_zpltmg.c
  core_zpltmg_chebvand.c
  core_zpltmg_circul.c
  core_zpltmg_condex.c
  core_zpltmg_fiedler.c
  core_zpltmg_hankel.c
  core_zpltmg_toeppd.c
  core_zpotrf.c
  core_zshift.c
  core_zssssm.c
  core_zsymm.c
  core_zsyr2k.c
  core_zsyrk.c
  core_zsyssq.c
  core_ztradd.c
  core_ztrasm.c
  core_ztrdv.c
  core_ztrmdm.c
  core_ztrmm.c
  core_ztrsm.c
  core_ztrssq.c
  core_ztrtri.c
  core_ztslqt.c
  core_ztsmlq.c
  core_ztsmlq_hetra1.c
  core_ztsmqr.c
  core_ztsmqr_corner.c
  core_ztsmqr_hetra1.c
  core_ztsqrt.c
  core_ztstrf.c
  core_zttlqt.c
  core_zttmlq.c
  core_zttmqr.c
  core_zttqrt.c
  core_zunmlq.c
  core_zunmqr.c
)
set(ZSOURCES_SDCZ core_zgessq.c )
set(ZSOURCES_CZ core_dlag2z.c )
set(PSOURCES
    gkkleader.c
    primes.c
    descriptor.c
    global.c)
if(DPLASMA_HAVE_CUDA)
set(ZHEADERS_CUDA "")
set(ZSOURCES_CUDA dplasma_cuda_ztsmqr.c)
endif()

include(PrecisionGenerator)
# reset variables
set(generated_cores "")
set(generated_cores_cz "")
set(generated_cores_sdcz "")
set(generated_cores_headers "")
set(generated_cores_cuda "")

### Generate the dplasma_cores for all required precisions
precisions_rules_py(generated_cores
    "${ZSOURCES}" PRECISIONS "${DPLASMA_PRECISIONS}")

### Generate the files with with non-selectable precisions
precisions_rules_py(generated_cores_cz
    "${ZSOURCES_CZ}" PRECISIONS "c;z")

precisions_rules_py(generated_cores_sdcz
    "${ZSOURCES_SDCZ}" PRECISIONS "s;d;c;z")

### generate the dplasma_cores headers for all precisions
precisions_rules_py(generated_cores_headers
                    ${ZHEADERS} ${ZHEADERS_CUDA}
                    PRECISIONS "s;d;c;z")
add_custom_target(dplasma_cores_includes ALL SOURCES
                  ${generated_cores_headers} )

### Publish the documented files
add_documented_files(PROJECT DPLASMA DIR ${CMAKE_CURRENT_BINARY_DIR} FILES ${generated_cores} ${generated_cores_sdcz} ${generated_cores_cz} ${generated_cores_headers})
add_documented_files(PROJECT DPLASMA DIR ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PSOURCES})

### Generate the dplasma_cores object library
# We cannot do a simple target_sources because of a bug in CMake <3.18 where the
# GENERATED property has a directory visibility
if(NOT TARGET dplasma_cores)
  add_library(dplasma_cores OBJECT ${PSOURCES} ${generated_cores} ${generated_cores_sdcz} ${generated_cores_cz})
endif(NOT TARGET dplasma_cores)
add_dependencies(dplasma_cores dplasma_includes dplasma_cores_includes)

set_target_properties(dplasma_cores PROPERTIES ENABLE_EXPORTS True)
set_target_properties(dplasma_cores PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
target_include_directories(dplasma_cores
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  $<$<NOT:${DPLASMA_BUILD_INPLACE}>:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(dplasma_cores
  PUBLIC
  PaRSEC::parsec
  LAPACKE::LAPACKE
  m)

# Integrate the output into the main library
target_sources(dplasma PRIVATE $<TARGET_OBJECTS:dplasma_cores>)

if( DPLASMA_HAVE_CUDA )
  # generate the cores_cuda library (hooks to cublas)
  precisions_rules_py(generated_cores_cuda
                      ${ZSOURCES_CUDA}
                      PRECISIONS "${DPLASMA_PRECISIONS}")

  if( NOT "${generated_cores_cuda}" STREQUAL "")
    add_documented_files(PROJECT DPLASMA DIR ${CMAKE_CURRENT_BINARY_DIR} FILES ${generated_cores_cuda})

    # Create a separate object library so that we can set cuda specific flags for these objects.
    add_library(dplasma_cores_cuda OBJECT ${generated_cores_cuda})

    add_dependencies(dplasma_cores_cuda dplasma_includes
                                        dplasma_cores_includes)
    set_target_properties(dplasma_cores_cuda PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
    target_include_directories(dplasma_cores_cuda
      PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR}
      $<$<NOT:${DPLASMA_BUILD_INPLACE}>:${CMAKE_CURRENT_SOURCE_DIR}>)
    target_link_libraries(dplasma_cores_cuda
      PRIVATE
      PaRSEC::parsec
      LAPACKE::LAPACKE
      m
      CUDA::cublas)

    # dplasma_cores is an object library, and linking an object library from an object library is 
    # currently broken (CMake 3.12). So we pull-in the sources of dplasma_cores_cuda directly.
    target_sources(dplasma PRIVATE $<TARGET_OBJECTS:dplasma_cores_cuda>)

    # Ability to generate .cu files into _smXY.so has been removed because it was unused and broke.
    # If needed again, use enable_language(CUDA) and add .cu files to add_library/target_sources, with
    # the PROPERTY CUDA_ARCHITECTURES set appropriately on the target library. Do not use find_package(CUDA).

  endif( NOT "${generated_cores_cuda}" STREQUAL "")
endif( DPLASMA_HAVE_CUDA )

