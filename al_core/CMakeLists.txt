project(al_core)
cmake_minimum_required(VERSION 3.0)

MESSAGE("***************************")
MESSAGE("***** bulding CORE ********")
MESSAGE("***************************")

FILE(GLOB SRC_LIST
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

if(UNIX)
  SET(CMAKE_C_COMPILER "/usr/bin/clang")
  SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

if (WIN32)

else()
  SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -DBOOST_LOG_DYN_LINK")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

if (WIN32)
  # SET Boost dir
  # SET(BOOST_ROOT "C:\\local\\boost_1_61_0")
  # SET(BOOST_LIBRARYDIR "C:\\local\\boost_1_61_0\\lib32-msvc-14.0")
  # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
  INCLUDE_DIRECTORIES( ../side-libs/boost_1_64_0 )
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  FIND_PACKAGE( Boost COMPONENTS system filesystem thread log REQUIRED )
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
endif()

INCLUDE_DIRECTORIES( ../al_utils/src )
INCLUDE_DIRECTORIES( ./src/ )
INCLUDE_DIRECTORIES( ./src )
INCLUDE_DIRECTORIES( include )
INCLUDE_DIRECTORIES( ../include )
INCLUDE_DIRECTORIES( ../vendor/Boost.DLL/include/ )

# TODO: temporary decision
if(APPLE)
  INCLUDE_DIRECTORIES(/opt/X11/include)
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
endif()

if (WIN32)

else()
  SET(progdll_dbg_libs
    ${Boost_SYSTEM_LIBRARY_DEBUG}
    ${Boost_FILESYSTEM_LIBRARY_DEBUG}
    ${Boost_LOG_LIBRARY_DEBUG})

  SET(progdll_opt_libs
    ${Boost_SYSTEM_LIBRARY_RELEASE}
    ${Boost_FILESYSTEM_LIBRARY_RELEASE}
    ${Boost_LOG_LIBRARY_RELEASE})

    INCLUDE(./cmake/FindJsoncpp.cmake)
    INCLUDE_DIRECTORIES( ${JSONCPP_INCLUDE_DIR} )
endif()

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_LIST})

if (APPLE)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${QUICKTIME}
    -ldl
  )
elseif(UNIX)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${JSONCPP_LIBRARY}
    -lal_utils
    -lboost_log_setup
    "/home/xors/workspace/lib/webrtc-checkout/src/out/Default/libprotobuf_lite.so"
    -ldl
  )
elseif(WIN32)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/al_utils.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/system/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_system-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/filesystem/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_filesystem-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/date_time/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_date_time-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/chrono/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_chrono-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/thread/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_thread-vc140-mt-1_64.lib
  )
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})
