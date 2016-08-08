project(al_ui.run)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRC_LIST
  src/*.cpp
  src/*.h
  src/*.cc
  src/**/*.cpp
  src/**/*.h
  src/**/*.cc
)

file(COPY res/cto_intro.png DESTINATION .)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)

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

# set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
# set_target_properties("al_ui.run" PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")

FIND_PACKAGE( SDL2 REQUIRED )
FIND_PACKAGE( Boost COMPONENTS system filesystem thread REQUIRED )
if (APPLE)
  FIND_PACKAGE( glm REQUIRED )
  FIND_LIBRARY( QUICKTIME QTKit )
endif()
FIND_PACKAGE( GLEW REQUIRED )

# used for testing now
FIND_PACKAGE( PNG REQUIRED )


INCLUDE( ${CMAKE_ROOT}/Modules/FindOpenGL.cmake )

INCLUDE_DIRECTORIES( ${SDL2_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( include )
INCLUDE_DIRECTORIES( ../include )
INCLUDE_DIRECTORIES( ../vendor/Boost.DLL/include/ )
INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ./src/ )
INCLUDE_DIRECTORIES( ${PNG_INCLUDE_DIR} )
if (GLEW_FOUND)
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

# TODO: temporary decision
if(APPLE)
  INCLUDE_DIRECTORIES(/opt/X11/include)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST})

if (APPLE)
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${PNG_LIBRARIES}
    ${QUICKTIME}
    -ldl)
else()
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    -lpng
    -ldl)
endif()
# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
