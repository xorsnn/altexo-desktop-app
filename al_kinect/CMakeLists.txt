project(al_kinect_1)
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

find_package( Boost COMPONENTS system filesystem thread REQUIRED )
# find_package(libfreenect REQUIRED)

include(./cmake/Findlibusb-1.0.cmake)

include_directories(include)
include_directories(../include)
include_directories(../vendor/Boost.DLL/include/)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
# include_directories(/usr/include/libfreenect/)
# include_directories(${LIBFREENECT_INCLUDE_DIR})
# message("--------------")
# message(${LIBFREENECT_INCLUDE_DIR})
# message("--------------")
if (APPLE)
  include_directories(/usr/local/Cellar/libfreenect/0.5.1/include/libfreenect/)
  link_directories(/usr/local/Cellar/libfreenect/0.5.1/lib)
elseif(UNIX)
  include_directories(/usr/include/libfreenect/)
endif()

include_directories(${LIBUSB_1_INCLUDE_DIRS})

# add_executable(${PROJECT_NAME} ${SRC_LIST})
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${LIBUSB_1_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  # ${FREENECT_LIBRARIES}
  -lfreenect -ldl)

# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
