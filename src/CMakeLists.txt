cmake_minimum_required(VERSION 2.8)
project(bone C)
# set option
option(DEF_NGLOBAL "DEF_NGLOBAL" OFF)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()
# set compiler
if(LINUX)
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER gcc++)
else()
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()
# define macros
if(DEF_NGLOBAL)
    set(NGLOBAL "-DNGLOBAL=2")
else()
    set(NGLOBAL "")
endif()
# build option
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
if(APPLE)
  set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address -O0 -DDEBUG -DVMDEBUG -MMD -Wall -Werror")
elseif(LINUX)
  set(CMAKE_C_FLAGS_DEBUG "-d -O0 -std=c99 -DDEBUG -DVMDEBUG -MMD -Wall")
endif()
set(CMAKE_C_FLAGS_RELEASE "-g -O2 -MMD -w -DNDEBUG")
# file location
set(BONE_EXECUTABLE "bone" CACHE STRING "executable file name")
set(BONE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bone CACHE PATH "Path to install")
set(BONE_IGNORE_COMPILE "CMakeCCompilerId.c" "feature_tests.c")
# include path, library path
if (WIN32)
    set(GLIB_LIBRARY_DIRS "C:/msys64/mingw64/bin" CACHE PATH "glib library dirs")
    set(GLIB_INCLUDE_DIRS "C:/msys64/mingw64/include/glib-2.0" "C:/msys64/mingw64/lib/glib-2.0/include" CACHE PATH "glib include dirs")
    set(GLIB_LIBRARIES "" CACHE FILEPATH "glib libraries")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLIB REQUIRED glib-2.0)
endif (WIN32)

if(NOT APPLE)
    if(LINUX AND NOT IS_DIRECTORY ${GLIB_LIBRARY_DIRS})
    set(GLIB_LIBRARY_DIRS /usr/lib)
    endif()
endif()
# config.h
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BONE_BUILD_VERSION "DEBUG vFirst")
elseif(CMAKE_BUILD_TYPE STREQUAL "RELEASE" OR CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BONE_BUILD_VERSION "RELEASE vFirst")
else()
    message(FATAL_ERROR "please fix build type: ${CMAKE_BUILD_TYPE}")
endif ()
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD)
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN)
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/bone/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/bone/config.h)

message(STATUS "GLIB_LIBRARY_DIRS=${GLIB_LIBRARY_DIRS}")
message(STATUS "GLIB_INCLUDE_DIRS=${GLIB_INCLUDE_DIRS}")
message(STATUS "GLIB_LIBRARIES=${GLIB_LIBRARIES}")
message(STATUS "BONE_INSTALL_DIR=${BONE_INSTALL_DIR}")
message(STATUS "HAVE_UNISTD=${HAVE_UNISTD}")
message(STATUS "HAVE_DLFCN=${HAVE_DLFCN}")
message(STATUS "HAVE_WINDOWS=${HAVE_WINDOWS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")

add_custom_target(
    myTest
    COMMAND pwd
    COMMAND ./bone
    COMMENT [[!! running tests !!]]
    WORKING_DIRECTORY ../bin
)

install(CODE "execute_process(COMMAND doxygen)")
install(
    DIRECTORY ./../bin
    USE_SOURCE_PERMISSIONS
    DESTINATION ${BONE_INSTALL_DIR}
)
install(
    DIRECTORY ./../doc
    USE_SOURCE_PERMISSIONS
    DESTINATION ${BONE_INSTALL_DIR}
)
install(
    DIRECTORY ./../lib
    USE_SOURCE_PERMISSIONS
    DESTINATION ${BONE_INSTALL_DIR}
)
install(
    DIRECTORY bone
    USE_SOURCE_PERMISSIONS
    DESTINATION ${BONE_INSTALL_DIR}/include
    FILES_MATCHING PATTERN "*.h"
)
add_custom_target(_add_path
    COMMAND printf "%s %s" export PATH=${BONE_INSTALL_DIR}/bin:$$ >> ~/.bash_profile
    COMMAND echo "PATH" >> ~/.bash_profile
    COMMAND source ~/.bash_profile
)
add_custom_target(_uninstall rm -r ${BONE_INSTALL_DIR})
add_custom_target(_clean rm -rf ./CMakeCache.txt && rm -r CMakeFiles)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "**/*.c")
foreach(e IN LISTS SOURCES)
    foreach(pat IN LISTS BONE_IGNORE_COMPILE)
        if(${e} MATCHES ${pat})
            message("remove compile target: ${e}")
            list(REMOVE_ITEM SOURCES ${e})
            break()
        endif()
    endforeach()
endforeach()

link_directories (
    ${GLIB_LIBRARY_DIRS}
)
if(NOT BONE_RUN AND NOT BONE_SHARED AND NOT BONE_STATIC)
    message(FATAL_ERROR "should add in any options: -DBONE_RUN=1, -DBONE_SHARED=1, -DBONE_STATIC=1")
endif()

if(BONE_RUN)
    add_executable(${BONE_EXECUTABLE} ${SOURCES})
endif()
if(BONE_SHARED)
    add_library(${BONE_EXECUTABLE} SHARED ${SOURCES})
endif()
if(BONE_STATIC)
    add_library(${BONE_EXECUTABLE} STATIC ${SOURCES})
endif()
add_definitions (
    ${GLIB_CFLAGS_OTHER}
    ${NGLOBAL}
)
target_include_directories(
    ${BONE_EXECUTABLE}
    PRIVATE
    ${GLIB_INCLUDE_DIRS}
)
target_link_libraries(
    ${BONE_EXECUTABLE}
    PRIVATE
    ${GLIB_LIBRARIES}
    ${CMAKE_DL_LIBS}
    m
)
set_target_properties(bone
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib"
    LIBRARY_OUTPUT_DIRECTORY "../lib"
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)
