cmake_minimum_required(VERSION 3.2.2)

project(@PROJECT_NAME@-pre-configure)

include(ExternalProject)

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY @CMAKE_MSVC_RUNTIME_LIBRARY@ CACHE INTERNAL "")
endif()

ExternalProject_Add(gperftools
  PREFIX            gperftools
  GIT_REPOSITORY    https://github.com/gperftools/gperftools
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gperftools-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gperftools-build"
  CMAKE_CACHE_ARGS
    "-DCMAKE_BUILD_TYPE:STRING=@CMAKE_BUILD_TYPE@"
    "-DCMAKE_INSTALL_PREFIX:STRING=@gperftools_INSTALL_PREFIX@"
    "-DBUILD_TESTING:BOOL=OFF"
    "-DGPERFTOOLS_BUILD_STATIC:BOOL=ON"
    "-Dgperftools_build_minimal:BOOL=ON"
)

ExternalProject_Add(protobuf
  PREFIX            protobuf
  GIT_REPOSITORY    https://github.com/protocolbuffers/protobuf
  GIT_TAG           v3.19.4
  SOURCE_SUBDIR     cmake
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/protobuf-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/protobuf-build"
  CMAKE_CACHE_ARGS
    "-DCMAKE_BUILD_TYPE:STRING=@CMAKE_BUILD_TYPE@"
    "-DCMAKE_INSTALL_PREFIX:STRING=@protobuf_INSTALL_PREFIX@"
    "-Dprotobuf_WITH_ZLIB:BOOL=ON"
    "-Dprotobuf_BUILD_TESTS:BOOL=OFF"
    "-Dprotobuf_BUILD_EXAMPLES:BOOL=OFF"
    "-Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF"
    "-Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=ON"
)

ExternalProject_Add(gRPC
  PREFIX            grpc
  GIT_REPOSITORY    https://github.com/grpc/grpc
  GIT_TAG           v1.46.1
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/grpc-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/grpc-build"
  CMAKE_CACHE_ARGS
    "-DCMAKE_BUILD_TYPE:STRING=@CMAKE_BUILD_TYPE@"
    "-DCMAKE_INSTALL_PREFIX:STRING=@gRPC_INSTALL_PREFIX@"
    "-DgRPC_INSTALL=ON"
    "-DgRPC_BUILD_TESTS:BOOL=OFF"
    "-DgRPC_BUILD_EXAMPLES:BOOL=OFF"
    "-DgRPC_BUILD_SHARED_LIBS:BOOL=OFF"
    "-DgRPC_MSVC_STATIC_RUNTIME:BOOL=ON"
)
