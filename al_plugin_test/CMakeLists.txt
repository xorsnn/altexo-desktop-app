project(al_plugin_test)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.cc
  src/**/*.cpp
  src/**/*.h
  src/**/*.cc
)

# set Boost dir
set(BOOST_ROOT "C:\\local\\boost_1_61_0")
set(BOOST_LIBRARYDIR "C:\\local\\boost_1_61_0\\lib32-msvc-14.0")

# common settings (Boost libraries)
if (MSVC)
  # Enable the static libraries on Windows
  foreach (flag_var
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    # update to the static version of the run time library
    string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
  endforeach()

  set(CMAKE_C_STANDARD_LIBRARIES "")
  set(CMAKE_CXX_STANDARD_LIBRARIES "")
endif()

# enable boost static flag
unset(Boost_LIBRARIES)
set(Boost_USE_STATIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

# boost components
find_package(Boost REQUIRED system filesystem)

include_directories(../include/)
include_directories(
  "${Boost_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES(./src/)

set(progdll_dbg_libs
  "${Boost_SYSTEM_LIBRARY_DEBUG}"
  "${Boost_FILESYSTEM_LIBRARY_DEBUG}")

set(progdll_opt_libs
  "${Boost_SYSTEM_LIBRARY_RELEASE}"
  "${Boost_FILESYSTEM_LIBRARY_RELEASE}")

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

# target_link_libraries(${PROJECT_NAME}
#   debug "${progdll_dbg_libs}"
#   optimized "${progdll_opt_libs}"
#   ProgPlugin)
