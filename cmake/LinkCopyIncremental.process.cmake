cmake_minimum_required(VERSION 3.25...3.30)

block()
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
  #REM: Default MODE to 'link'
  set(MODE "link")
endif()

#REM: little helpers
function(_lci_make_parent_dir #[[dst]] path)
  get_filename_component(_dir "${path}" DIRECTORY)
  if(NOT _dir STREQUAL "")
    file(MAKE_DIRECTORY "${_dir}")
  endif()
endfunction()

function(_lci_clean #[[dst]] path)
  if(EXISTS "${path}" OR IS_SYMLINK "${path}")
    file(REMOVE_RECURSE "${path}")
  endif()
endfunction()

function(_lci_copy src dst)
  file(MAKE_DIRECTORY "${dst}")

  file(
    GLOB_RECURSE _src_rel_entries
    RELATIVE "${src}" "${src}/*")

  foreach(_src_rel IN LISTS _src_rel_entries)
    set(_src_path "${src}/${_src_rel}")
    set(_dst_path "${dst}/${_src_rel}")

    if(IS_SYMLINK "${_src_path}" AND NOT IS_DIRECTORY "${_src_path}")
      #REM: Symlink to a file: copy the file contents through the link.
      if(IS_DIRECTORY "${_dst_path}" AND NOT IS_SYMLINK "${_dst_path}")
        _lci_clean("${_dst_path}")
      endif()

      _lci_make_parent_dir("${_dst_path}")
      file(
        COPY_FILE "${_src_path}" "${_dst_path}"
        ONLY_IF_DIFFERENT)

    elseif(IS_DIRECTORY "${_src_path}" AND NOT IS_SYMLINK "${_src_path}")
      #REM: Real directory in SRC. Replace a non-directory at 'DST' if present.
      if(EXISTS "${_dst_path}" AND NOT IS_DIRECTORY "${_dst_path}")
        _lci_clean("${_dst_path}")
      endif()

      file(MAKE_DIRECTORY "${_dst_path}")

    else()
      #REM: Regular file. Replace a directory at 'DST' if present.
      if(IS_DIRECTORY "${_dst_path}" AND NOT IS_SYMLINK "${_dst_path}")
        _lci_clean("${_dst_path}")
      endif()

      _lci_make_parent_dir("${_dst_path}")
      file(
        COPY_FILE "${_src_path}" "${_dst_path}"
        ONLY_IF_DIFFERENT)
    endif()
  endforeach()

  #REM: Prune 'DST' entries that are gone from 'SRC'
  file(
    GLOB_RECURSE _dst_rel_entries
    RELATIVE "${dst}" "${dst}/*")

  foreach(_dst_rel IN LISTS _dst_rel_entries)
    set(_src_path "${src}/${_dst_rel}")
    set(_dst_path "${dst}/${_dst_rel}")

    #REM: Is '_dst_rel' already remove AND it is not a 
    #REM: SYMLINK/JUNCTION in the 'DST'
    if(NOT EXISTS "${_dst_path}" AND NOT IS_SYMLINK "${_dst_path}")
      continue()
    endif()

    #REM: Is '_dst_rel' still exist OR a SYMLINK/JUNCTION in the 'SRC'
    if(EXISTS "${_src_path}" OR IS_SYMLINK "${_src_path}")
      continue()
    endif()

    #REM: Stale - remove: Symlinks | directories | files
    if(IS_SYMLINK "${_dst_path}")
      file(REMOVE "${_dst_path}")
    elseif(IS_DIRECTORY "${_dst_path}")
      _lci_clean("${_dst_path}")
    else()
      file(REMOVE "${_dst_path}")
    endif()
  endforeach()
endfunction()

function(_lci_link src dst)
  _lci_make_parent_dir("${dst}")

  if(WIN32)
    #REM: Already a symlink/junction - safe to clear and retry directly.
    if(IS_SYMLINK "${dst}")
      _lci_clean("${dst}")

      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)

      if(NOT _rc EQUAL 0)
        #REM: Fall back to COPY if SYMLINK/JUNCTION failed
        _lci_copy("${src}" "${dst}")
      endif()

    #REM: Real directory at 'DST' (previous incremental copy) - preserve it.
    elseif(EXISTS "${dst}")
      set(_dst_bak "${dst}.bak_lci")

      if(EXISTS "${_dst_bak}" OR IS_SYMLINK "${_dst_bak}")
        #REM: If previous artifacts still exist clean it.
        _lci_clean("${_dst_bak}")
      endif()

      #REM: Preserve current dst path
      file(RENAME "${dst}" "${_dst_bak}")

      #REM: Try to SYMLINK/JUNCTION the said target path (src--->dst)
      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)

      if(_rc EQUAL 0)
        #REM: Junction succeeded - the backup is no longer needed.
        _lci_clean("${_dst_bak}")
      else()
        #REM: Junction failed - rollback/restore 

        #REM: Remove failed SYMLINK/JUNCTION artifacts
        if(EXISTS "${dst}" OR IS_SYMLINK "${dst}")
          _lci_clean("${dst}")
        endif()
        
        #REM: Rollback/restore the previous backup,
        #REM: then fall back to an incremental copy into it.
        file(RENAME "${_dst_bak}" "${dst}")
        _lci_copy("${src}" "${dst}")
      endif()

    #REM: Brand-new destination.
    else()
      execute_process(
        COMMAND cmd /c mklink /J "${dst}" "${src}"
        RESULT_VARIABLE _rc OUTPUT_QUIET ERROR_QUIET)

      if(NOT _rc EQUAL 0)
        _lci_copy("${src}" "${dst}")
      endif()
    endif()

  else()
    #REM: Unix: symlinks rarely fail, but handle filesystems that refuse them.
    _lci_clean("${dst}")
    file(
      CREATE_LINK "${src}" "${dst}" SYMBOLIC
      RESULT _rc)

    if(NOT _rc EQUAL 0)
      message(WARNING
        "LinkCopyIncremental: symlink failed (${_rc}); falling back to copy.")
      _lci_copy("${src}" "${dst}")
    endif()
  endif()
endfunction()

#REM: Dispatch
string(TOUPPER "${MODE}" _mode_uc)

if(_mode_uc STREQUAL "LINK")
  _lci_link("${SRC}" "${DST}")
elseif(_mode_uc STREQUAL "COPY")
  _lci_copy("${SRC}" "${DST}")
else()
  message(FATAL_ERROR
    "LinkCopyIncremental: unknown MODE='${MODE}' (expected 'LINK' or 'COPY')")
endif()
endblock()