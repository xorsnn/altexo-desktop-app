project(altexo_sdk)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.cc
  src/**/*.cpp
  src/**/*.h
  src/**/*.cc
)

INCLUDE_DIRECTORIES(./src/)

##
# Compiler flags
#
if(UNIX)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g  -pipe -fno-rtti -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -O0 -std=c++11 -Wall -W -D_REENTRANT")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m64 -pipe -fno-rtti -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -O0 -std=c++11 -Wall -W -D_REENTRANT")
endif()

##
# USEFUL VARS
#
if (APPLE)
  set(WEBRTCTRUNK "/Volumes/MAC/workspace/WEBRTC/webrtc/src")
elseif(UNIX)
  set(WEBRTCTRUNK "/home/xors/workspace/ALTEXO/webrtc/src")
endif()
set(WEBRTCBUILD "${WEBRTCTRUNK}/out/Release")

##
# DEFINES
#
add_definitions(-DUNIX)
if(UNIX)
  add_definitions(-DLINUX)
endif()
add_definitions(-DWEBRTC_POSIX)
add_definitions(-DPOSIX)

# Set output dir
# set(BIN_DIR ${${PROJECT_NAME}_SOURCE_DIR}/../../al_build/al_ui)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
# add_executable(${PROJECT_NAME} ${SRC_LIST})

if (APPLE)
  # set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  # set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  # set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  # set(CMAKE_MACOSX_RPATH 1)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
elseif(UNIX)
  # compile and link for 64 bit mode
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
endif()
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
if (APPLE)
  include(al-webrtc-deps-mac.cmake)
elseif(UNIX)
  include(al-webrtc-deps.cmake)
endif()

find_package( Boost COMPONENTS system filesystem thread REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

if (APPLE)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC
    ${WEBRTC_LIBS}
    -lpthread
    -ldl
    -lexpat
    -lm -ldl
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  )
elseif(UNIX)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC
    -Wl,--start-group
    ${WEBRTC_LIBS}
    -Wl,--end-group
    -lpthread
    -lX11 -lXfixes -lXi -lXcomposite -lXext -lXrender -ldl -lrt -lexpat -lXrandr -lm -ldl
    boost_thread boost_system
  )
endif()
