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

INCLUDE_DIRECTORIES(./src/)

##
# Compiler flags
#
if(UNIX)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics -fvisibility-inlines-hidden -Wno-undefined-bool-conversion -Wno-tautological-undefined-compare -std=gnu++11 -fno-rtti -fno-exceptions -Wnon-virtual-dtor -Woverloaded-virtual")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

##
# USEFUL VARS
#

# ##
# # DEFINES
# #
# if (UNIX)
#   add_definitions(-DLINUX)
#   add_definitions(-DV8_DEPRECATION_WARNINGS)
#   add_definitions(-DUSE_UDEV)
#   add_definitions(-DUI_COMPOSITOR_IMAGE_TRANSPORT)
#   add_definitions(-DUSE_AURA=1)
#   add_definitions(-DUSE_PANGO=1)
#   add_definitions(-DUSE_CAIRO=1)
#   add_definitions(-DUSE_GLIB=1)
#   add_definitions(-DUSE_NSS_CERTS=1)
#   add_definitions(-DUSE_X11=1)
#   add_definitions(-DFULL_SAFE_BROWSING)
#   add_definitions(-DSAFE_BROWSING_CSD)
#   add_definitions(-DSAFE_BROWSING_DB_LOCAL)
#   add_definitions(-DCHROMIUM_BUILD)
#   add_definitions(-DENABLE_MEDIA_ROUTER=1)
#   add_definitions(-DFIELDTRIAL_TESTING_ENABLED)
#   add_definitions(-DCR_CLANG_REVISION=289575-1)
#   add_definitions(-D_FILE_OFFSET_BITS=64)
#   add_definitions(-D_LARGEFILE_SOURCE)
#   add_definitions(-D_LARGEFILE64_SOURCE)
#   add_definitions(-D__STDC_CONSTANT_MACROS)
#   add_definitions(-D__STDC_FORMAT_MACROS)
#   add_definitions(-DCOMPONENT_BUILD)
#   add_definitions(-D_DEBUG)
#   add_definitions(-DDYNAMIC_ANNOTATIONS_ENABLED=1)
#   add_definitions(-DWTF_USE_DYNAMIC_ANNOTATIONS=1)
#   add_definitions(-D_GLIBCXX_DEBUG=1)
#   add_definitions(-DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE)
#   add_definitions(-DEXPAT_RELATIVE_PATH)
#   add_definitions(-DWEBRTC_POSIX)
#   add_definitions(-DWEBRTC_LINUX)
#   add_definitions(-DWEBRTC_BUILD_LIBEVENT)
# endif()

# if (UNIX)
#   if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
#     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
#   endif()
# endif()


# # enable boost static flag
# unset(Boost_LIBRARIES)
# set(Boost_USE_STATIC ON)
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME ON)
#
# ##
# # LIBRARIES
# #
# if(UNIX)
#   include(al-webrtc-deps.cmake)
#   include(al-webrtc-obj-deps.cmake)
#   include(al-webrtc-deps-simple.cmake)
# endif()

##
# Creating library
#
if (UNIX)
  add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
  # add_executable(${PROJECT_NAME} ${SRC_LIST} ${OBJ_DEPS})
  # add_executable(${PROJECT_NAME} ${SRC_LIST} ${OBJ_DEPS})
  # add_executable(${PROJECT_NAME} ${SRC_LIST})
endif()

##
# INCLUDE
#
include_directories(.)
# include_directories(${WEBRTCTRUNK}/)
# include_directories(${WEBRTCTRUNK}/third_party/)
# include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/overrides/include)
# include_directories(${WEBRTCTRUNK}/third_party/jsoncpp/source/include)
# include_directories(${WEBRTCTRUNK}/chromium/src/third_party/libyuv/include)
include_directories(../include/)
# INCLUDE_DIRECTORIES( ../al_utils/src )
# INCLUDE_DIRECTORIES( ../al_sdk_2_1/src )


# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/third_party/libyuv/include)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/gtk-2.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/lib/x86_64-linux-gnu/gtk-2.0/include)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/gio-unix-2.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/cairo)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/pango-1.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/atk-1.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/cairo)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/pixman-1)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/libpng12)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/gdk-pixbuf-2.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/libpng12)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/pango-1.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/freetype2)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/include/glib-2.0)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/build/linux/debian_wheezy_amd64-sysroot/usr/lib/x86_64-linux-gnu/glib-2.0/include)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/third_party/jsoncpp/overrides/include)
# INCLUDE_DIRECTORIES(${WEBRTCTRUNK}/third_party/jsoncpp/source/include)


# if(UNIX)
#   FIND_PACKAGE( Boost COMPONENTS system filesystem thread REQUIRED )
#   INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
# endif()


# if(UNIX)
#   TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC
#     # -Wl,--start-group
#     # ${WEBRTC_LIBS}
#     # -Wl,--end-group
#     # # ${OBJ_DEPS}
#     # -lpthread
#     # -lX11
#     # -lXcomposite
#     # -lXext
#     # -lXrender
#     # -ldl
#     # -lrt
#     # -lexpat
#     # -lm
#     # -ldl
#     # # -ljsoncpp
#     # "/home/xors/workspace/ALTEXO/altexo/build/libal_utils.so"
#     # # "/home/xors/workspace/ALTEXO/altexo/build/libaltexo_sdk_tester.so"
#     # # boost_thread boost_system
#     # "/home/xors/workspace/lib/webrtc-checkout/src/out/Default/libboringssl.so"
#     # "/home/xors/workspace/lib/webrtc-checkout/src/out/Default/libprotobuf_lite.so"
#     # -lgmodule-2.0
#     # -lgtk-x11-2.0
#     # -lgdk-x11-2.0
#     # -lpangocairo-1.0
#     # -latk-1.0
#     # -lcairo
#     # -lgdk_pixbuf-2.0
#     # -lgio-2.0
#     # -lpangoft2-1.0
#     # -lpango-1.0
#     # -lgobject-2.0
#     # -lfreetype
#     # -lfontconfig
#     # -lgthread-2.0
#     # -lglib-2.0
#   )
# endif()
