project(al_utils)
cmake_minimum_required(VERSION 2.8)

message("***************************")
message("***** bulding utils *******")
message("***************************")

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


if (WIN32)
else()
# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
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
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# # set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
# # set_target_properties("al_ui.run" PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
# if (WIN32)
#   # set Boost dir
#   set(BOOST_ROOT "C:\\local\\boost_1_61_0")
#   set(BOOST_LIBRARYDIR "C:\\local\\boost_1_61_0\\lib32-msvc-14.0")
# else()
# if (APPLE)
#   FIND_PACKAGE( glm REQUIRED )
#   FIND_LIBRARY( QUICKTIME QTKit )
# endif()
# FIND_PACKAGE( SDL2 REQUIRED )
# FIND_PACKAGE( GLEW REQUIRED )
# # used for testing now
# FIND_PACKAGE( PNG REQUIRED )
# endif()
#
# FIND_PACKAGE( Boost COMPONENTS system filesystem thread log REQUIRED )
#
# if (APPLE)
#   FIND_PACKAGE( OpenGL REQUIRED )
# elseif(UNIX)
#   INCLUDE( ${CMAKE_ROOT}/Modules/FindOpenGL.cmake )
# endif()
#
# if(UNIX)
# INCLUDE_DIRECTORIES( ${SDL2_INCLUDE_DIR} )
# INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIRS} )
# INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
# INCLUDE_DIRECTORIES( ${PNG_INCLUDE_DIR} )
# if (GLEW_FOUND)
#     INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
#     link_libraries(${GLEW_LIBRARIES})
# endif()
# # win32
# else()
#   INCLUDE_DIRECTORIES( "D:\\WORKSPACE\\BUILD_ALTEXO\\glew-2.0.0\\include" )
#   INCLUDE_DIRECTORIES( "D:\\WORKSPACE\\BUILD_ALTEXO\\SDL2-2.0.4\\build\\include" )
#   INCLUDE_DIRECTORIES( "D:\\WORKSPACE\\BUILD_ALTEXO\\glm" )
#   # INCLUDE_DIRECTORIES( "D:\\WORKSPACE\\BUILD_ALTEXO\\lpng1624\\build" )
# endif()
#
# INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#
# INCLUDE_DIRECTORIES( ./src/ )
# INCLUDE_DIRECTORIES( ./src )
# INCLUDE_DIRECTORIES( include )
# INCLUDE_DIRECTORIES( ../include )
# INCLUDE_DIRECTORIES( ../vendor/Boost.DLL/include/ )
#
# # TODO: temporary decision
# if(APPLE)
#   INCLUDE_DIRECTORIES(/opt/X11/include)
#   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
#   set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
# endif()
#
# set(progdll_dbg_libs
#   ${Boost_SYSTEM_LIBRARY_DEBUG}
#   ${Boost_FILESYSTEM_LIBRARY_DEBUG}
#   ${Boost_LOG_LIBRARY_DEBUG})
#   # "${Boost_THREAD_LIBRARY_DEBUG}")
#
# set(progdll_opt_libs
#   ${Boost_SYSTEM_LIBRARY_RELEASE}
#   ${Boost_FILESYSTEM_LIBRARY_RELEASE}
#   ${Boost_LOG_LIBRARY_RELEASE})
#   # "${Boost_THREAD_LIBRARY_RELEASE}")
#
# message(${progdll_opt_libs})

# add_executable(${PROJECT_NAME} ${SRC_LIST})
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

# if (APPLE)
#   target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
#     ${OPENGL_LIBRARIES}
#     ${Boost_FILESYSTEM_LIBRARY}
#     ${Boost_SYSTEM_LIBRARY}
#     ${Boost_THREAD_LIBRARY}
#     ${PNG_LIBRARIES}
#     ${QUICKTIME}
#     -ldl)
# elseif(UNIX)
#   target_link_libraries(${PROJECT_NAME}
#     ${SDL2_LIBRARY}
#     ${OPENGL_LIBRARIES}
#     ${Boost_FILESYSTEM_LIBRARY}
#     ${Boost_SYSTEM_LIBRARY}
#     ${Boost_THREAD_LIBRARY}
#     ${Boost_LOG_LIBRARY}
#     -lboost_log_setup
#     -lpng
#     -ldl)
# elseif(WIN32)
#   target_link_libraries(${PROJECT_NAME}
#     "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_system-vc140-mt-1_61.lib"
#     "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_filesystem-vc140-mt-1_61.lib"
#     # debug "${progdll_dbg_libs}"
#     # optimized "${progdll_opt_libs}"
#
#     "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_date_time-vc140-mt-1_61.lib"
#     "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_chrono-vc140-mt-1_61.lib"
#     "C:\\local\\boost_1_61_0\\lib32-msvc-14.0\\libboost_thread-vc140-mt-1_61.lib"
#
#     "D:\\WORKSPACE\\BUILD_ALTEXO\\SDL2-2.0.4\\build\\Release\\SDL2.lib"
#     "D:\\WORKSPACE\\BUILD_ALTEXO\\SDL2-2.0.4\\build\\Release\\SDL2main.lib"
#     "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.10586.0\\um\\x86\\WinMM.Lib"
#     "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.10586.0\\um\\x86\\Imm32.Lib"
#     "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.10586.0\\um\\x86\\Version.Lib"
#     "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.10586.0\\um\\x86\\OpenGL32.Lib"
#   )
# endif()
