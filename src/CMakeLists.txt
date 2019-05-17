cmake_minimum_required(VERSION 3.5)
project(bone C)

set(CMAKE_CXX_COMPILER clang)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address -O0 -DDEBUG -DVMDEBUG -MMD -Wall")
set(CMAKE_C_FLAGS_RELEASE "-g -O2 -MMD -w -DNDEBUG")
set(BONE_EXECUTABLE "bone" CACHE STRING "executable file name")
set(BONE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bone CACHE PATH "Path to install")
set(BONE_IGNORE_COMPILE "CMakeCCompilerId.c" "feature_tests.c")

if (WIN32)
    set(GLIB_LIBRARY_DIRS "C:/msys64/mingw64/bin" CACHE PATH "glib library dirs")
    set(GLIB_INCLUDE_DIRS "C:/msys64/mingw64/include/glib-2.0" "C:/msys64/mingw64/lib/glib-2.0/include" CACHE PATH "glib include dirs")
    set(GLIB_LIBRARIES "" CACHE FILEPATH "glib libraries")
    set(CUNIT_LIBRARY "C:/msys64/mingw64/bin" CACHE PATH "cunit library dirs")
    set(CUNIT_INCLUDE_DIR "C:/msys64/mingw64/include/CUnit" CACHE PATH "cunit include dirs")
else()
    find_package(PkgConfig REQUIRED)
    find_path(CUNIT_INCLUDE_DIR NAMES CUnit/CUnit.h)
    find_library(CUNIT_LIBRARY NAMES cunit)
    find_package_handle_standard_args(CUnit DEFAULT_MSG CUNIT_LIBRARY CUNIT_INCLUDE_DIR)

    pkg_check_modules(GLIB REQUIRED glib-2.0)
endif (WIN32)
message(STATUS "GLIB_LIBRARY_DIRS=${GLIB_LIBRARY_DIRS}")
message(STATUS "GLIB_INCLUDE_DIRS=${GLIB_INCLUDE_DIRS}")
message(STATUS "GLIB_LIBRARIES=${GLIB_LIBRARIES}")
message(STATUS "CUNIT_INCLUDE_DIR=${CUNIT_INCLUDE_DIR}")
message(STATUS "CUNIT_LIBRARY=${CUNIT_LIBRARY}")
message(STATUS "BONE_INSTALL_DIR=${BONE_INSTALL_DIR}")


# enable_testing()
# add_test(
#     NAME test
#     COMMAND ruby auto/test.rb
#     #CONFIGURATIONS Debug
#     WORKING_DIRECTORY .
# )

install(DIRECTORY DESTINATION ${BONE_INSTALL_DIR})
install(
    DIRECTORY ./../bin
    DESTINATION ${BONE_INSTALL_DIR}
)
install(
    DIRECTORY ./../doc
    DESTINATION ${BONE_INSTALL_DIR}
)
add_custom_target(_uninstall rm -r ${BONE_INSTALL_DIR})

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
if(BONE_RUN)
    add_executable(${BONE_EXECUTABLE} ${SOURCES})
endif()
if(BONE_SHARED)
    add_library(${BONE_EXECUTABLE} SHARED ${SOURCES})
endif()
if(BONE_STATIC)
    add_library(${BONE_EXECUTABLE} STATIC ${SOURCES})
endif()
add_definitions (${GLIB_CFLAGS_OTHER})
target_include_directories(
    ${BONE_EXECUTABLE}
    PRIVATE
    ${GLIB_INCLUDE_DIRS}
    ${CUNIT_INCLUDE_DIR}
)
target_link_libraries(
    ${BONE_EXECUTABLE}
    PRIVATE
    ${GLIB_LIBRARIES}
    ${CUNIT_LIBRARY}
)
set_target_properties(bone
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib"
    LIBRARY_OUTPUT_DIRECTORY "../lib"
    RUNTIME_OUTPUT_DIRECTORY "../lib"
)