cmake_minimum_required(VERSION 3.25...3.30)

block()

#[[
#REM: Usage:

add_custom_target(${project.TARGET_BASENAME}_resources
  COMMAND "${CMAKE_COMMAND}"
    "-DSRC=${project.SRC_MAIN_RESOURCES_DIR}"
    "-DDST=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
    "-DMODE=LINK"
    "-DFILES_MATCHING=REGEX_EXCLUDE:\\..*keep$"
    -P "${PROJECT_SOURCE_DIR}/cmake/LinkCopyIncremental.process.cmake"
  VERBATIM
  COMMENT "[main](build-time) ${project.TARGET_BASENAME}_resources; Update Resource Paths")
]]

#REM: --- validate ---

if(NOT DEFINED SRC OR NOT DEFINED DST)
  message(FATAL_ERROR
    "LinkCopyIncremental: 'SRC' and 'DST' are required")
endif()

if(NOT IS_ABSOLUTE "${SRC}")
  message(FATAL_ERROR
    "LinkCopyIncremental: 'SRC' must be absolute: (${SRC})")
endif()

if(NOT IS_ABSOLUTE "${DST}")
  message(FATAL_ERROR
    "LinkCopyIncremental: 'DST' must be absolute: (${DST})")
endif()

if(NOT EXISTS "${SRC}")
  message(WARNING
    "LinkCopyIncremental: 'SRC' path not found: (${SRC})")
endif()

if(NOT DEFINED MODE)
  set(MODE "link")
endif()

#REM: --- FILES_MATCHING ---

set(_lci_include_rx)
set(_lci_exclude_rx)

if(DEFINED FILES_MATCHING)
  foreach(_rule IN LISTS FILES_MATCHING)
    if(_rule STREQUAL "")
      continue()
    endif()
    if(_rule MATCHES "^REGEX_INCLUDE:(.+)$")
      list(APPEND _lci_include_rx "${CMAKE_MATCH_1}")
    elseif(_rule MATCHES "^REGEX_EXCLUDE:(.+)$")
      list(APPEND _lci_exclude_rx "${CMAKE_MATCH_1}")
    else()
      message(WARNING
        "LinkCopyIncremental: unknown FILES_MATCHING rule: '${_rule}'")
    endif()
  endforeach()
endif()

#REM: --- helpers ---

function(_lci_make_parent_dir path)
  get_filename_component(_dir "${path}" DIRECTORY)
  if(NOT _dir STREQUAL "")
    file(MAKE_DIRECTORY "${_dir}")
  endif()
endfunction()

function(_lci_clean path)
  if(EXISTS "${path}" OR IS_SYMLINK "${path}")
    file(REMOVE_RECURSE "${path}")
  endif()
endfunction()

function(_lci_is_excluded rel out_var)
  foreach(_rx IN LISTS _lci_exclude_rx)
    if("${rel}" MATCHES "${_rx}")
      set(${out_var} TRUE PARENT_SCOPE)
      return()
    endif()
  endforeach()
  set(${out_var} FALSE PARENT_SCOPE)
endfunction()

function(_lci_is_included rel out_var)
  if(NOT _lci_include_rx)
    set(${out_var} TRUE PARENT_SCOPE)
    return()
  endif()
  foreach(_rx IN LISTS _lci_include_rx)
    if("${rel}" MATCHES "${_rx}")
      set(${out_var} TRUE PARENT_SCOPE)
      return()
    endif()
  endforeach()
  set(${out_var} FALSE PARENT_SCOPE)
endfunction()

#REM: Collect SRC-relative entries under `root`, honoring INCLUDE/EXCLUDE and
#REM: deliberately *not* descending into directory symlinks (they are emitted
#REM: as leaf entries; `_lci_copy` skips them with a warning).
function(_lci_collect root out_var)
  set(_out)
  set(_queue "")

  file(GLOB _top LIST_DIRECTORIES true "${root}/*")
  foreach(_e IN LISTS _top)
    file(RELATIVE_PATH _rel "${root}" "${_e}")
    list(APPEND _queue "${_rel}")
  endforeach()

  while(_queue)
    list(POP_FRONT _queue _rel)
    set(_abs "${root}/${_rel}")

    _lci_is_excluded("${_rel}" _ex)
    if(_ex)
      continue()
    endif()

    if(IS_DIRECTORY "${_abs}" AND NOT IS_SYMLINK "${_abs}")
      list(APPEND _out "${_rel}")
      file(GLOB _children LIST_DIRECTORIES true "${_abs}/*")
      foreach(_c IN LISTS _children)
        file(RELATIVE_PATH _rel2 "${root}" "${_c}")
        list(APPEND _queue "${_rel2}")
      endforeach()
    else()
      _lci_is_included("${_rel}" _ok)
      if(_ok)
        list(APPEND _out "${_rel}")
      endif()
    endif()
  endwhile()

  set(${out_var} "${_out}" PARENT_SCOPE)
endfunction()

#REM: --- copy ---

function(_lci_copy src dst)
  #REM: If a previous LINK pass left a symlink/junction here, drop it first so
  #REM: that the prune walk below cannot trample SRC through it.
  if(IS_SYMLINK "${dst}")
    _lci_clean("${dst}")
  endif()
  file(MAKE_DIRECTORY "${dst}")

  _lci_collect("${src}" _src_rel)

  foreach(_rel IN LISTS _src_rel)
    set(_src_path "${src}/${_rel}")
    set(_dst_path "${dst}/${_rel}")

    if(IS_DIRECTORY "${_src_path}" AND NOT IS_SYMLINK "${_src_path}")
      if(EXISTS "${_dst_path}" AND NOT IS_DIRECTORY "${_dst_path}")
        _lci_clean("${_dst_path}")
      endif()
      file(MAKE_DIRECTORY "${_dst_path}")

    elseif(IS_SYMLINK "${_src_path}" AND IS_DIRECTORY "${_src_path}")
      message(WARNING
        "LinkCopyIncremental: symlinked directory in SRC is skipped: "
        "(${_src_path})")
      continue()

    else()
      if(IS_DIRECTORY "${_dst_path}" AND NOT IS_SYMLINK "${_dst_path}")
        _lci_clean("${_dst_path}")
      endif()
      _lci_make_parent_dir("${_dst_path}")
      file(COPY_FILE "${_src_path}" "${_dst_path}" ONLY_IF_DIFFERENT)
    endif()
  endforeach()

  #REM: Prune anything in DST that is either stale or filtered out of SRC.
  file(GLOB_RECURSE _dst_rel
       LIST_DIRECTORIES true
       RELATIVE "${dst}" "${dst}/*")

  foreach(_rel IN LISTS _dst_rel)
    if(_rel IN_LIST _src_rel)
      continue()
    endif()
    set(_p "${dst}/${_rel}")
    if(EXISTS "${_p}" OR IS_SYMLINK "${_p}")
      file(REMOVE_RECURSE "${_p}")
    endif()
  endforeach()
endfunction()

#REM: --- link ---

function(_lci_link src dst)
  _lci_make_parent_dir("${dst}")

  if(WIN32)
    if(IS_SYMLINK "${dst}")
      _lci_clean("${dst}")
      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)
      if(NOT _rc EQUAL 0)
        #REM: mklink can fail *after* creating a partial reparse point.
        if(EXISTS "${dst}" OR IS_SYMLINK "${dst}")
          _lci_clean("${dst}")
        endif()
        _lci_copy("${src}" "${dst}")
      endif()

    elseif(EXISTS "${dst}")
      set(_dst_bak "${dst}.bak_lci")
      if(EXISTS "${_dst_bak}" OR IS_SYMLINK "${_dst_bak}")
        _lci_clean("${_dst_bak}")
      endif()
      file(RENAME "${dst}" "${_dst_bak}")

      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)

      if(_rc EQUAL 0)
        _lci_clean("${_dst_bak}")
      else()
        if(EXISTS "${dst}" OR IS_SYMLINK "${dst}")
          _lci_clean("${dst}")
        endif()
        file(RENAME "${_dst_bak}" "${dst}")
        _lci_copy("${src}" "${dst}")
      endif()

    else()
      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)
      if(NOT _rc EQUAL 0)
        if(EXISTS "${dst}" OR IS_SYMLINK "${dst}")
          _lci_clean("${dst}")
        endif()
        _lci_copy("${src}" "${dst}")
      endif()
    endif()

  else()
    #REM: Unix: early-out if the existing symlink already points at SRC.
    if(IS_SYMLINK "${dst}" AND EXISTS "${src}")
      file(READ_SYMLINK "${dst}" _cur)
      get_filename_component(_dst_dir "${dst}" DIRECTORY)
      get_filename_component(_cur_abs "${_cur}" ABSOLUTE BASE_DIR "${_dst_dir}")
      if(EXISTS "${_cur_abs}")
        file(REAL_PATH "${src}"     _src_real)
        file(REAL_PATH "${_cur_abs}" _cur_real)
        if(_cur_real STREQUAL _src_real)
          return()
        endif()
      endif()
    endif()

    _lci_clean("${dst}")
    file(CREATE_LINK "${src}" "${dst}" SYMBOLIC RESULT _rc)
    if(NOT _rc EQUAL 0)
      message(WARNING
        "LinkCopyIncremental: symlink failed (${_rc}); falling back to copy.")
      _lci_copy("${src}" "${dst}")
    endif()
  endif()
endfunction()

#REM:--- dispatch ---

string(TOUPPER "${MODE}" _mode_uc)

#REM: A junction/symlink cannot honor FILES_MATCHING (it exposes the whole tree).
#REM: Degrade to COPY rather than silently violate the caller's intent.
if(_mode_uc STREQUAL "LINK" AND (_lci_include_rx OR _lci_exclude_rx))
  message(WARNING
    "LinkCopyIncremental: FILES_MATCHING is set, but MODE=LINK uses a "
    "junction/symlink and cannot filter. Falling back to MODE=COPY.")
  set(_mode_uc "COPY")
endif()

if(_mode_uc STREQUAL "LINK")
  _lci_link("${SRC}" "${DST}")
elseif(_mode_uc STREQUAL "COPY")
  _lci_copy("${SRC}" "${DST}")
else()
  message(FATAL_ERROR
    "LinkCopyIncremental: unknown MODE='${MODE}' (expected 'LINK' or 'COPY')")
endif()

endblock()