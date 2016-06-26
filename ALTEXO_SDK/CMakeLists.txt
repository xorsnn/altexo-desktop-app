project(altexo_sdk)
cmake_minimum_required(VERSION 2.8)
# aux_source_directory(. SRC_LIST)
# aux_source_directory(./conductor SRC_LIST_NEST)
# MESSAGE(${SRC_LIST})
# MESSAGE(${SRC_LIST_NEST})
set(SOURCES
  src/almanager.cpp
  src/conductor/conductor.cc
  src/conductor/defaults.cc
  src/conductor/peer_connection_client.cc
  src/alvideorenderer.cpp
  src/conductor/aldatamanager.cpp
  src/almanagerinterface.cpp
  src/alvideocapturer.cpp
  src/alvideocapturerthread.cpp
)
set(HEADERS
  src/almanager.h
  src/conductor/conductor.h
  src/customsocketserver.h
  src/conductor/defaults.h
  src/conductor/peer_connection_client.h
  src/alcallback.h
  src/alvideorenderer.h
  src/conductor/aldatamanager.h
  src/almanagerinterface.h
  src/alvideocapturer.h
  src/alvideocapturerthread.h
)
set(SRC_LIST
  ${SOURCES}
  ${HEADERS}
)
INCLUDE_DIRECTORIES(./src/)

##
# Compiler flags
#
# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m64 -pipe -fno-rtti -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -O0 -std=c++11 -Wall -W -D_REENTRANT")

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

##
# USEFUL VARS
#
set(WEBRTCTRUNK "/home/xors/workspace/ALTEXO/webrtc/src")
set(WEBRTCBUILD "${WEBRTCTRUNK}/out/Release")

##
# DEFINES
#
add_definitions(-DUNIX)
add_definitions(-DLINUX)
add_definitions(-DWEBRTC_POSIX)
add_definitions(-DPOSIX)

# Set output dir
set(BIN_DIR ${${PROJECT_NAME}_SOURCE_DIR}/../../al_build/al_ui)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
# add_executable(${PROJECT_NAME} ${SRC_LIST})

# compile and link for 64 bit mode
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
set(CMAKE_AUTOMOC ON)

##
# INCLUDE
#
include_directories(.)
include_directories(${WEBRTCTRUNK}/)
include_directories(${WEBRTCTRUNK}/chromium/src/third_party/jsoncpp/source/include/)
include_directories(${WEBRTCTRUNK}/third_party/)
include_directories(${WEBRTCTRUNK}/third_party/webrtc/)
include_directories(${WEBRTCTRUNK}/webrtc/net/third_party/nss/ssl)
include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/overrides/include)
include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/source/include)
include_directories(../include/)

##
# LIBRARIES
#
include(al-webrtc-deps.cmake)
find_package( Boost )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC
  -Wl,--start-group
  ${WEBRTC_LIBS}
  -Wl,--end-group
  -lpthread
  -lX11 -lXfixes -lXi -lXcomposite -lXext -lXrender -ldl -lrt -lexpat -lXrandr -lm -ldl
  boost_thread boost_system
)
