cmake_minimum_required(VERSION 2.8)
project(varAppName C)

set(CMAKE_CXX_COMPILER clang)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_C_FLAGS_DEBUG "-g -coverage -fsanitize=address -O0 -DDEBUG -MMD -Wall")
set(CMAKE_C_FLAGS_RELEASE "-g -O2 -MMD -w -DNDEBUG")
set(varExecutable "varAppName" CACHE STRING "executable file name")
set(varInstallDir ${CMAKE_INSTALL_PREFIX}/varAppName CACHE PATH "Path to install")
set(varIgnoreCompile "CMakeCCompilerId.c" "feature_tests.c")

if (WIN32)
    set(GLIB_LIBRARY_DIRS "C:/msys64/mingw64/bin" CACHE PATH "glib library dirs")
    set(GLIB_INCLUDE_DIRS "C:/msys64/mingw64/include/glib-2.0" "C:/msys64/mingw64/lib/glib-2.0/include" CACHE PATH "glib include dirs")
    set(GLIB_LIBRARIES "" CACHE FILEPATH "glib libraries")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLIB REQUIRED glib-2.0)
endif (WIN32)
message(STATUS "GLIB_LIBRARY_DIRS=${GLIB_LIBRARY_DIRS}")
message(STATUS "GLIB_INCLUDE_DIRS=${GLIB_INCLUDE_DIRS}")
message(STATUS "GLIB_LIBRARIES=${GLIB_LIBRARIES}")
message(STATUS "varInstallDir=${varInstallDir}")


# enable_testing()
# add_test(
#     NAME test
#     COMMAND ruby auto/test.rb
#     #CONFIGURATIONS Debug
#     WORKING_DIRECTORY .
# )

install(
    DIRECTORY ./../bin
    DESTINATION ${varInstallDir}
)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "**/*.c")
foreach(e IN LISTS SOURCES)
    foreach(pat IN LISTS varIgnoreCompile)
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
add_executable(${varExecutable} ${SOURCES})
add_definitions (${GLIB_CFLAGS_OTHER})
target_include_directories(
    ${varExecutable}
    PRIVATE
    ${GLIB_INCLUDE_DIRS}
)
target_link_libraries(
    ${varExecutable}
    PRIVATE
    ${GLIB_LIBRARIES}
)