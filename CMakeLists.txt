cmake_minimum_required(VERSION 3.0)
project (rootproject)
SET( CMAKE_VERBOSE_MAKEFILE on )

if(APPLE)
#   # set(CMAKE_C_COMPILER "/Volumes/MAC/workspace/WEBRTC/webrtc/src/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang")
#   # set(CMAKE_CXX_COMPILER "/Volumes/MAC/workspace/WEBRTC/webrtc/src/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++")
elseif(UNIX)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  # set(CMAKE_CXX_COMPILER "/usr/bin/g++")
  # set(CMAKE_C_COMPILER "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/third_party/llvm-build/Release+Asserts/bin/clang")
  # set(CMAKE_CXX_COMPILER "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/third_party/llvm-build/Release+Asserts/bin/clang++")
endif()

# add_definitions(-DAL_DEBUG)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(al_utils)

# Standard apps

add_subdirectory(al_core)
add_subdirectory(al_ui)
# add_subdirectory(al_json_rpc_client)
add_subdirectory(al_json_rpc_client_2)
# add_subdirectory(al_kinect)
# add_subdirectory(al_fake_sensor)
# add_subdirectory(al_realsense)
