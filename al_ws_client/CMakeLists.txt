project(ws_client)
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

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")


# Set output dir
# set(BIN_DIR ${${PROJECT_NAME}_SOURCE_DIR}/../../al_build/al_ui)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
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
# set(ENABLE_CXX_EXCEPTIONS 0)

include_directories(include)
include_directories(/usr/include)

# TODO: environment dependent
if (APPLE)
  set(LIB_CPR_PATH "/Volumes/MAC/workspace/cpr")
  set(OPENSSL_LIBRARIES "/usr/local/Cellar/openssl/1.0.2h_1/lib")
  set(OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl/1.0.2h_1/")
  include_directories("/usr/local/Cellar/openssl/1.0.2h_1/include/")
elseif(UNIX)
  set(LIB_CPR_PATH "/home/xors/workspace/ALTEXO/cpr")
endif()

include_directories("${LIB_CPR_PATH}/include")
include_directories(../include/)

find_package( Boost COMPONENTS system thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# add_executable(${PROJECT_NAME} ${SRC_LIST})
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
  ${LIB_CPR_PATH}/build/lib/libcpr.a
  -lpthread -lssl -lcrypto -lcurl -lwebsockets
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
