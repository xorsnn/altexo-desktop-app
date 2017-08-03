project(al_utils)
cmake_minimum_required(VERSION 3.0)

message("***************************")
message("***** bulding utils *******")
message("***************************")

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.hpp
  src/*.cc
  src/**/*.cpp
  src/**/*.h
  src/**/*.hpp
  src/**/*.cc
  src/**/*.c
)

if (WIN32)
  INCLUDE_DIRECTORIES( ../side-libs/boost_1_64_0 )
else()
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
  set(CMAKE_AUTOMOC ON)
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -DBOOST_LOG_DYN_LINK")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

if (WIN32)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
  # BASE_NAME ${PROJECT_NAME}
  # EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORTS
  # EXPORT_FILE_NAME ${PROJECT_NAME}_EXPORTS.h
  # STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
)
