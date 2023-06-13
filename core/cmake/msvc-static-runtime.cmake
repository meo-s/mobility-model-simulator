if(WIN32)
  foreach(flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
    if(NOT ${flag_var} MATCHES "/MT")
      set(${flag_var} "${${flag_var}} /MT")
      if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(${flag_var} "${${flag_var}}d")
      endif()
    endif()
  endforeach()
endif()
