project(altexo_sdk)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

##
# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

##
# USEFUL VARS
#
set(WEBRTCTRUNK "/home/xors/workspace/ALTEXO/webrtc/src")

##
# DEFINES
#
add_definitions(-DLINUX)
add_definitions(-DWEBRTC_POSIX)
add_definitions(-DPOSIX)

# Set output dir
set(BIN_DIR ${${PROJECT_NAME}_SOURCE_DIR}/../../al_build/al_ui)

add_executable(${PROJECT_NAME} ${SRC_LIST})

include_directories(.)
include_directories(${WEBRTCTRUNK}/)
include_directories(${WEBRTCTRUNK}/chromium/src/third_party/jsoncpp/source/include/)
include_directories(${WEBRTCTRUNK}/third_party/)
include_directories(${WEBRTCTRUNK}/third_party/webrtc/)
include_directories(${WEBRTCTRUNK}/webrtc/net/third_party/nss/ssl)
include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/overrides/include)
include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/source/include)
