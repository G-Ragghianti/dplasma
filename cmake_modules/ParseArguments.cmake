#
# Copyright (c) 2011-2023 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
#
macro(PARSE_ARGUMENTS prefix arg_names option_names)
  set(DEFAULT_ARGS)
  foreach(arg_name ${arg_names})
    set(${prefix}_${arg_name})
  endforeach(arg_name)
  foreach(option ${option_names})
    set(${prefix}_${option} FALSE)
  endforeach(option)

  set(current_arg_name DEFAULT_ARGS)
  set(current_arg_list)
  foreach(arg ${ARGN})
    set(larg_names ${arg_names})
    list(FIND larg_names "${arg}" is_arg_name)
    if (is_arg_name GREATER -1)
      set(${prefix}_${current_arg_name} ${current_arg_list})
      set(current_arg_name ${arg})
      set(current_arg_list)
    else (is_arg_name GREATER -1)
      set(loption_names ${option_names})
      list(FIND loption_names "${arg}" is_option)
      if (is_option GREATER -1)
             set(${prefix}_${arg} TRUE)
      else (is_option GREATER -1)
             set(current_arg_list ${current_arg_list} ${arg})
      endif (is_option GREATER -1)
    endif (is_arg_name GREATER -1)
  endforeach(arg)
  set(${prefix}_${current_arg_name} ${current_arg_list})
endmacro(PARSE_ARGUMENTS)

macro(CAR var)
  set(${var} ${ARGV1})
endmacro(CAR)

macro(CDR var junk)
  set(${var} ${ARGN})
endmacro(CDR)
