include_guard(GLOBAL)

function(load_that_env filepath)
  if(NOT EXISTS "${filepath}")
    message(FATAL_ERROR 
      "File not found: '${filepath}'")
  endif()

  file(STRINGS "${filepath}" lines
    ENCODING UTF-8)
  
  foreach(line IN LISTS lines)

    string(STRIP "${line}" line)

    if(line STREQUAL "" OR line MATCHES "^#") 
      continue()
    endif()
 
    if(NOT line MATCHES "^([^=]+)=(.*)$")
      continue()
    endif()

    set(key "${CMAKE_MATCH_1}")
    set(value "${CMAKE_MATCH_2}")

    string(STRIP "${key}" key)
    string(STRIP "${value}" value)

    if(key STREQUAL "")
      continue()
    endif()

    if(value MATCHES "^\"(.*)\"$" 
      OR value MATCHES "^'(.*)'$")
      set(value "${CMAKE_MATCH_1}")
    endif()

    set("${key}" "${value}" PARENT_SCOPE)

  endforeach()
endfunction()