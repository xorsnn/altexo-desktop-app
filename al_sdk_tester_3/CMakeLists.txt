project(altexo_sdk_tester.run)
cmake_minimum_required(VERSION 2.8)

message("********************************")
message("***** bulding SDK tester *******")
message("********************************")

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.cc
  src/**/*.cpp
  src/**/*.h
  src/**/*.cc
)

if(UNIX)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

################################################################################
# Compiler flags
################################################################################
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -fPIC")
endif()

################################################################################
# Find packages
################################################################################
FIND_PACKAGE( Boost COMPONENTS system filesystem thread log REQUIRED )

################################################################################
# Creating library
################################################################################
if (UNIX)
  add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()

################################################################################
# Include directories
################################################################################
INCLUDE_DIRECTORIES(./src/)
INCLUDE_DIRECTORIES(../include/)
INCLUDE_DIRECTORIES( ../vendor/Boost.DLL/include/ )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

################################################################################
# Linking
################################################################################
target_link_libraries(${PROJECT_NAME}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  "/home/xors/workspace/lib/webrtc-checkout/src/out/Default/libprotobuf_lite.so"
  -ldl
)
