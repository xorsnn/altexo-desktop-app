project(al_kinect_1_win.run)
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

# find_package( Boost COMPONENTS system filesystem thread REQUIRED )

include_directories("C:\\Program Files\\Microsoft SDKs\\Kinect\\v1.8\\inc")
include_directories("C:\\local\\boost_1_61_0")
# include_directories(include)
# include_directories(../include)
# include_directories(../vendor/Boost.DLL/include/)
# INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
"C:\\Program Files\\Microsoft SDKs\\Kinect\\v1.8\\lib\\x86\\Kinect10.lib"
"C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_system-vc140-mt-1_61.lib"
# "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\boost_filesystem-vc140-mt-1_61.lib"
"C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_thread-vc140-mt-1_61.lib"
"C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_date_time-vc140-mt-1_61.lib"
"C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_chrono-vc140-mt-1_61.lib"

)
  # ${LIBUSB_1_LIBRARIES}
#   ${Boost_FILESYSTEM_LIBRARY}
#   ${Boost_SYSTEM_LIBRARY}
#   ${Boost_THREAD_LIBRARY}
#   -lfreenect -ldl)
