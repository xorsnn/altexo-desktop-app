project(json_rpc_client_2)
cmake_minimum_required(VERSION 2.8)

message("********************************")
message("***** bulding RPC client 2 *****")
message("********************************")

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.cc
  src/*.c
  src/**/*.cpp
  src/**/*.h
  src/**/*.cc
  src/**/*.c
)
INCLUDE_DIRECTORIES(./src/)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -fPIC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(LWS_OPENSSL_SUPPORT 1)

if(WIN32)
  SET( JSONCPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/jsoncpp_install/include )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/cpr-1.3.0/include )
  INCLUDE_DIRECTORIES( ../side-libs/boost_1_64_0 )
  INCLUDE_DIRECTORIES( ../side-libs/libwebsockets_install/include )
  INCLUDE_DIRECTORIES( ../side-libs/openssl/include )
  INCLUDE_DIRECTORIES( ../side-libs/libwebsockets/win32port/win32helpers )
elseif(UNIX)
  SET( LIB_CPR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/cpr-1.3.0 )
  INCLUDE_DIRECTORIES( ${LIB_CPR_PATH}/include )
#   INCLUDE_DIRECTORIES( /usr/include )
  find_package( Boost COMPONENTS system thread REQUIRED )
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#   INCLUDE( ./cmake/FindJsoncpp.cmake )
endif()

# INCLUDE_DIRECTORIES( include )
# INCLUDE_DIRECTORIES( ../include/ )
INCLUDE_DIRECTORIES( ${JSONCPP_INCLUDE_DIR} )

# # TODO: environment dependent
# if (APPLE)
#   set( LIB_CPR_PATH "/Volumes/MAC/workspace/cpr" )
#   set( OPENSSL_LIBRARIES "/usr/local/Cellar/openssl/1.0.2h_1/lib" )
#   set( OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl/1.0.2h_1/" )
#   INCLUDE_DIRECTORIES("/usr/local/Cellar/openssl/1.0.2h_1/include/" )
#   INCLUDE_DIRECTORIES( /Volumes/MAC/workspace/libwebsockets/build )
#   INCLUDE_DIRECTORIES( /Volumes/MAC/workspace/libwebsockets/lib )
#   link_directories( /Volumes/MAC/workspace/libwebsockets/build/lib )
# elseif(UNIX)
# endif()

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/jsoncpp/build/src/lib_json/Release/jsoncpp.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/libwebsockets_install/lib/websockets_static.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/system/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_system-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/filesystem/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_filesystem-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/date_time/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_date_time-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/chrono/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_chrono-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/thread/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_thread-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/openssl/lib/libeay32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/openssl/lib/ssleay32.lib
    wsock32.lib
    ws2_32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/libwebsockets/build/lib/Release/zlib_internal.lib
  )
else()
  target_link_libraries(${PROJECT_NAME}
    ${LIB_CPR_PATH}/build/lib/libcpr.a
    # ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/cpr-1.3.0/build/lib/libcpr.a
    -lpthread -lssl -lcrypto -lcurl
    -lwebsockets
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${JSONCPP_LIBRARY}
  )
endif()
