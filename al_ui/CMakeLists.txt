project(al_ui.run)
cmake_minimum_required(VERSION 2.8)

MESSAGE("***************************")
MESSAGE("***** bulding UI **********")
MESSAGE("***************************")
SET( CMAKE_VERBOSE_MAKEFILE on )

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

file(GLOB SHADERS_LIST
  shaders/*.frag
  shaders/*.vert
)

if(UNIX)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  # set(CMAKE_CXX_COMPILER "/usr/bin/g++")
  # set(CMAKE_C_COMPILER "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/third_party/llvm-build/Release+Asserts/bin/clang")
  # set(CMAKE_CXX_COMPILER "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/third_party/llvm-build/Release+Asserts/bin/clang++")
endif()

file( COPY res/cto_intro.png DESTINATION . )

# shaders
if (WIN32)
  file( COPY ${SHADERS_LIST} DESTINATION ../Release/shaders )
else()
  file( COPY ${SHADERS_LIST} DESTINATION ../shaders )
endif()

if (WIN32)
else()
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -stdlib=libstdc++ -DBOOST_LOG_DYN_LINK")
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if (WIN32)
else()
  if (APPLE)
    FIND_PACKAGE( glm REQUIRED )
    FIND_LIBRARY( QUICKTIME QTKit )
  endif()
  FIND_PACKAGE( SDL2 REQUIRED )
  FIND_PACKAGE( GLEW REQUIRED )
  # used for testing now
  FIND_PACKAGE( PNG REQUIRED )
endif()

FIND_PACKAGE( Boost COMPONENTS system filesystem thread log REQUIRED )

if (APPLE)
  FIND_PACKAGE( OpenGL REQUIRED )
elseif(UNIX)
  INCLUDE( ${CMAKE_ROOT}/Modules/FindOpenGL.cmake )
endif()

if(UNIX)
  INCLUDE_DIRECTORIES( ${SDL2_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIRS} )
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${PNG_INCLUDE_DIR} )
  if (GLEW_FOUND)
      INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
      link_libraries(${GLEW_LIBRARIES})
  endif()
# win32
else()
  INCLUDE_DIRECTORIES( "D:\\WORKSPACE\\BUILD_ALTEXO\\glew-2.0.0\\include" )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/SDL2-2.0.5/build/include )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/SDL2-2.0.5/include )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/glm-0.9.8.4 )
endif()

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

INCLUDE_DIRECTORIES( ../al_utils/src )
INCLUDE_DIRECTORIES( ../al_core/src )
INCLUDE_DIRECTORIES( ./src/ )
INCLUDE_DIRECTORIES( ./src )
INCLUDE_DIRECTORIES( include )
INCLUDE_DIRECTORIES( ../include )
INCLUDE_DIRECTORIES( ../vendor/Boost.DLL/include/ )

# TODO: temporary decision
if(APPLE)
  INCLUDE_DIRECTORIES(/opt/X11/include)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
endif()

set(progdll_dbg_libs
  ${Boost_SYSTEM_LIBRARY_DEBUG}
  ${Boost_FILESYSTEM_LIBRARY_DEBUG}
  ${Boost_LOG_LIBRARY_DEBUG})
  # "${Boost_THREAD_LIBRARY_DEBUG}")

set(progdll_opt_libs
  ${Boost_SYSTEM_LIBRARY_RELEASE}
  ${Boost_FILESYSTEM_LIBRARY_RELEASE}
  ${Boost_LOG_LIBRARY_RELEASE})
  # "${Boost_THREAD_LIBRARY_RELEASE}")

MESSAGE( ${progdll_opt_libs} )

link_directories(${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_LIST})

if (APPLE)
  target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${PNG_LIBRARIES}
    ${QUICKTIME}
    -ldl)
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/out/Default/obj/third_party/protobuf/libprotobuf_lite.a"
    ${SDL2_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_LOG_LIBRARY}
    -lal_utils
    -lboost_log_setup
    -lpng
    -lal_core
    # "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/out/Default/obj/webrtc/examples/libaltexosdk.a"
    # "/home/xors/workspace/lib/webrtc-checkout/src/out/Default/libprotobuf_lite.so"
    # "/home/xors/workspace/lib/webrtc/webrtc-checkout/src/out/Default/obj/webrtc/examples/libaltexosdk.a"
    # -lssl
    # -lcrypto
    # -lstdc++
    -ldl)
elseif(WIN32)
  target_link_libraries(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/al_utils.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/al_core.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/system/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_system-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/filesystem/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_filesystem-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/date_time/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_date_time-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/chrono/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_chrono-vc140-mt-1_64.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/boost_1_64_0/bin.v2/libs/thread/build/msvc-14.0/rls/lnk-sttc/thrd-mlt/libboost_thread-vc140-mt-1_64.lib

    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/SDL2-2.0.5/build/Release/SDL2.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../side-libs/SDL2-2.0.5/build/Release/SDL2main.lib

    # "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.15063.0\\um\\x86\\WinMM.Lib"
    # "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.15063.0\\um\\x86\\Imm32.Lib"
    # "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.15063.0\\um\\x86\\Version.Lib"
    # "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.15063.0\\um\\x86\\OpenGL32.Lib"
    WinMM.Lib
    Imm32.Lib
    Version.Lib
    OpenGL32.Lib
  )
endif()
