cmake_minimum_required(VERSION 3.20.0)

include("FindPkgConfig")

project("mobmodel-simulator-core")

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT DEFINED CMAKE_BUILD_TYPE)
  option(CMAKE_BUILD_TYPE "Release")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d" CACHE INTERNAL "A postfix for Debug configuration.")
  endif()
  set(debug_postfix ${CMAKE_DEBUG_POSTFIX})
endif()

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  include("cmake/msvc-static-runtime.cmake")
endif()

set(gperftools_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/externals/gperftools${debug_postfix}-install")
set(protobuf_INSTALL_PREFIX   "${CMAKE_CURRENT_BINARY_DIR}/externals/protobuf${debug_postfix}-install")
set(gRPC_INSTALL_PREFIX       "${CMAKE_CURRENT_BINARY_DIR}/externals/grpc${debug_postfix}-install")

if((NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/externals") OR (${BUILD_EXTERNALS}))
  configure_file("cmake/pre-configure.cmake" "${CMAKE_CURRENT_BINARY_DIR}/externals/CMakeLists.txt" @ONLY)
  execute_process(COMMAND ${CMAKE_COMMAND}
    -G "${CMAKE_GENERATOR}" -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/externals"
  )
  execute_process(COMMAND ${CMAKE_COMMAND}
    --build . --config ${CMAKE_BUILD_TYPE} --parallel
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/externals"
  )
endif()

add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

file(GLOB libtcmalloc "${gperftools_INSTALL_PREFIX}/lib/*.a" "${gperftools_INSTALL_PREFIX}/lib/*.lib")
find_package(protobuf   HINTS "${protobuf_INSTALL_PREFIX}" REQUIRED)
find_package(absl       HINTS "${gRPC_INSTALL_PREFIX}" REQUIRED)
find_package(gRPC       HINTS "${gRPC_INSTALL_PREFIX}" REQUIRED)
target_link_libraries(${PROJECT_NAME} "${libtcmalloc}" protobuf::libprotobuf gRPC::grpc++)

set(tcmalloc_INCLUDE_DIRS "${gperftools_INSTALL_PREFIX}/include")
set(protobuf_INCLUDE_DIRS "${protobuf_INSTALL_PREFIX}/include")
set(gRPC_INCLUDE_DIRS     "${gRPC_INSTALL_PREFIX}/include")
target_include_directories(${PROJECT_NAME} PRIVATE ${tcmalloc_INCLUDE_DIRS} ${protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

message(STATUS "Compiling .proto files ...")
get_filename_component(protos_dir "${CMAKE_CURRENT_SOURCE_DIR}/../protos" REALPATH)
get_filename_component(protos_root_dir "${protos_dir}" DIRECTORY)
if(NOT EXISTS ${protos_dir})
//  message(FATAL_ERROR "Failed to find root directory of proto files.")
else()
  find_program(protoc NAMES protoc HINTS "${protobuf_INSTALL_PREFIX}/bin" REQUIRED)
  find_program(grpc_cpp_plugin NAMES grpc_cpp_plugin HINTS "${gRPC_INSTALL_PREFIX}/bin" REQUIRED)
  
  file(GLOB proto_files "${protos_dir}/*.proto")
  foreach(proto_file IN LISTS proto_files)
    unset(protoc_args)
    list(APPEND protoc_args "--proto_path=${protos_root_dir}" "--cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/src")
    # if(proto_file MATCHES "\\.service\\.proto$")
    list(APPEND protoc_args "--grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/src")
    list(APPEND protoc_args "--plugin=protoc-gen-grpc=${grpc_cpp_plugin}")
    # endif()

    execute_process(COMMAND ${protoc} ${protoc_args} "${proto_file}")
  endforeach()
endif()
message(STATUS "Compiling .proto files done")

file(GLOB_RECURSE src_files "src/*.cc" "src/*.hh" "src/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${src_files})
