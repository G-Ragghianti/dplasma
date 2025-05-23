#
# Copyright (c) 2009-2023 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
#

#
# PaRSEC Internal: generation of various floating point precision files from a template.
#
include(ParseArguments)
find_package(Python 3 COMPONENTS Interpreter REQUIRED QUIET)

if(NOT DEFINED PRECISIONGENERATOR_LOCATION)
  message(FATAL_ERROR "PRECISIONGENERATOR_LOCATION is not defined as it
  should")
endif(NOT DEFINED PRECISIONGENERATOR_LOCATION)

set(GENDEPENDENCIES  ${PRECISIONGENERATOR_LOCATION}/PrecisionDeps.py)
set(PRECISIONPP      ${PRECISIONGENERATOR_LOCATION}/PrecisionGenerator.py)
set(PRECISIONPP_subs ${PRECISIONGENERATOR_LOCATION}/subs.py)

#
# Generates a rule for every SOURCES file, to create the precisions in PRECISIONS. If TARGETDIR
# is not empty then all generated files will be prepended with the $TARGETDIR/.
# A new file is created, from a copy by default
# If the first precision is "/", all occurences of the basename in the file are remplaced by
# "pbasename" where p is the selected precision.
# the target receives a -DPRECISION_p in its cflags.
#
function(precisions_rules_py)
  PARSE_ARGUMENTS(PREC_RULE
    "TARGETDIR;PRECISIONS"
    ""
    ${ARGN})
  # The first is the output variable list
  CAR(OUTPUTLIST ${PREC_RULE_DEFAULT_ARGS})
  # Everything else should be source files.
  CDR(SOURCES ${PREC_RULE_DEFAULT_ARGS})
  message(STATUS "Generate precision dependencies in ${CMAKE_CURRENT_SOURCE_DIR}\t${OUTPUTLIST}")

  set(PRECISIONPP_arg "-P")
  # By default the TARGETDIR is the current binary directory
  if( "${PREC_RULE_TARGETDIR}" STREQUAL "" )
    set(PREC_RULE_TARGETDIR "./")
    set(PRECISIONPP_prefix "./")
  else( "${PREC_RULE_TARGETDIR}" STREQUAL "" )
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${PREC_RULE_TARGETDIR})
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PREC_RULE_TARGETDIR})
    endif(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${PREC_RULE_TARGETDIR})
    set(PRECISIONPP_prefix "${PREC_RULE_TARGETDIR}")
  endif( "${PREC_RULE_TARGETDIR}" STREQUAL "" )

  set(options_list "")
  foreach(prec_rules_PREC ${PREC_RULE_PRECISIONS})
    set(options_list "${options_list} ${prec_rules_PREC}")
  endforeach()

  set(sources_list "")
  foreach(_src ${SOURCES})
    set(sources_list "${sources_list} ${_src}")
  endforeach()

  set(gencmd ${Python_EXECUTABLE} ${GENDEPENDENCIES} -f "${sources_list}" -p "${options_list}" -s "${CMAKE_CURRENT_SOURCE_DIR}" ${PRECISIONPP_arg} ${PRECISIONPP_prefix})
  execute_process(COMMAND ${gencmd} OUTPUT_VARIABLE dependencies_list)

  foreach(_dependency ${dependencies_list})

    string(STRIP "${_dependency}" _dependency)
    string(COMPARE NOTEQUAL "${_dependency}" "" not_empty)
    if( not_empty )

      string(REGEX REPLACE "^(.*),(.*),(.*)$" "\\1" _dependency_INPUT "${_dependency}")
      set(_dependency_PREC   "${CMAKE_MATCH_2}")
      set(_dependency_OUTPUT "${CMAKE_MATCH_3}")

      set(pythoncmd ${Python_EXECUTABLE} ${PRECISIONPP} -f ${CMAKE_CURRENT_SOURCE_DIR}/${_dependency_INPUT} -p ${_dependency_PREC} ${PRECISIONPP_arg} ${PRECISIONPP_prefix})

      string(STRIP "${_dependency_OUTPUT}" _dependency_OUTPUT)
      string(COMPARE NOTEQUAL "${_dependency_OUTPUT}" "" got_file)

      # Force the copy of the original files in the binary_dir
      # for VPATH compilation
      if( NOT DPLASMA_BUILD_INPLACE )
        set(generate_out 1)
      else( NOT DPLASMA_BUILD_INPLACE )
        string(COMPARE NOTEQUAL "${_dependency_OUTPUT}" "${_dependency_INPUT}" generate_out )
      endif()

      # We generate a dependency only if a file will be generated
      if( got_file )
        if( generate_out )
          # the custom command is executed in CMAKE_CURRENT_BINARY_DIR
          add_custom_command(
            OUTPUT ${_dependency_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E remove -f ${_dependency_OUTPUT} && ${pythoncmd} && chmod a-w ${_dependency_OUTPUT}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_dependency_INPUT} ${PRECISIONPP} ${PRECISIONPP_subs})
          # Copy properties from the original to the generated
          foreach(_prop_name COMPILE_OPTIONS COMPILE_DEFINITIONS INCLUDE_DIRECTORIES)
            get_source_file_property(_prop_value ${CMAKE_CURRENT_SOURCE_DIR}/${_dependency_INPUT} ${_prop_name})
            if( _prop_value )
              set_property(SOURCE ${_dependency_OUTPUT} APPEND PROPERTY ${_prop_name} "${_prop_value}")
            endif()
          endforeach()

        endif( generate_out )
        # Remove existing PRECISION_x that may have been copied from the original
        get_source_file_property(_compile_defs ${_dependency_OUTPUT} COMPILE_DEFINITIONS)
        if( _compile_defs )
          foreach(_prec PRECISION_s PRECISION_d PRECISION_c PRECISION_z)
            list(FIND _compile_defs ${_prec} _index)
            if( ${_index} GREATER -1 )
              list(REMOVE_AT _compile_defs ${_index})
            endif()
          endforeach()
          set_source_files_properties(${_dependency_OUTPUT} PROPERTIES COMPILE_DEFINITIONS "${_compile_defs}")
        endif( _compile_defs )
        # Add (back?) the PRECISION_x definition
        set_source_files_properties(${_dependency_OUTPUT} APPEND PROPERTIES
            COMPILE_DEFINITIONS "PRECISION_${_dependency_PREC}"
            INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})
        list(APPEND outputlist "${_dependency_OUTPUT}")
      endif( got_file )
    endif()
  endforeach()

  #message(STATUS "Generate precision dependencies in ${CMAKE_CURRENT_SOURCE_DIR}\t${OUTPUTLIST} : Done")
  set(${OUTPUTLIST} ${outputlist} PARENT_SCOPE)
endfunction(precisions_rules_py)
