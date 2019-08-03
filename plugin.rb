require_relative "bone"
require 'fileutils'
require "open3"

os = Bone::os()
if os != :macosx
    puts("unsupported a your operating system")
    exit(0)
end

# create ../bone_embed
puts("create project directories")
cwd = Bone::check_cwd()
project_dir = Bone::unique_dir(File::dirname(cwd) + "/bone_plugin")
Dir.mkdir(project_dir)

# create ../bone_embed/src
src_dir = Bone::unique_dir(project_dir + "/src")
Dir.mkdir(src_dir)
src_gitig = Bone::unique_file(src_dir + "/.gitkeep")
FileUtils.touch(src_gitig)

# create ../bone_embed/src/plugin
src_app_dir = Bone::unique_dir(src_dir + "/plugin")
Dir.mkdir(src_app_dir)

# create ../bone_embed/bin
bin_dir = Bone::unique_dir(project_dir + "/bin")
Dir.mkdir(bin_dir)
bin_gitig = Bone::unique_file(bin_dir + "/.gitkeep")
FileUtils.touch(bin_gitig)

# create ../bone_embed/.gitignore
ignore_option = <<-EOS
CMakeLists.txt.user
CMakeCache.txt
CMakeFiles
CMakeScripts
Testing
Makefile
cmake_install.cmake
install_manifest.txt
compile_commands.json
CTestTestfile.cmake
_deps
EOS
project_gitig = Bone::unique_file(project_dir + "/.gitignore")
File.open(project_gitig, "w") do |fp|
    fp.write(ignore_option)
end

# create ../bone_embed/src/CMakeLists.txt
cmake_option = <<-EOS
cmake_minimum_required(VERSION 2.8)
project(varProjectName C)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

if(LINUX)
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER gcc++)
else()
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
if(APPLE)
  set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address -O0 -DDEBUG -DVMDEBUG -MMD -Wall")
elseif(LINUX)
  set(CMAKE_C_FLAGS_DEBUG "-d -O0 -std=c99 -DDEBUG -DVMDEBUG -MMD -Wall")
endif()
set(CMAKE_C_FLAGS_RELEASE "-g -O2 -MMD -w -DNDEBUG")
set(APP_EXECUTABLE "varProjectName" CACHE STRING "executable file name")
set(APP_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/varProjectName CACHE PATH "Path to install")
set(APP_IGNORE_COMPILE "CMakeCCompilerId.c" "feature_tests.c")
set(BONE_INCLUDE "your bone header" CACHE PATH "Path to bone header")
set(BONE_LIBRARY_DIRS "your bone library" CACHE PATH "Path to bone library")
set(BONE_LIBRARIES "bone")
message(STATUS "BONE_INCLUDE=${BONE_INCLUDE}")
message(STATUS "BONE_LIBRARY_DIRS=${BONE_LIBRARY_DIRS}")
message(STATUS "BONE_LIBRARIES=${BONE_LIBRARIES}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

if(NOT APPLE)
    if(LINUX AND NOT IS_DIRECTORY ${GLIB_LIBRARY_DIRS})
    set(GLIB_LIBRARY_DIRS /usr/lib)
    endif()
endif()

add_custom_target(_uninstall rm -r ${APP_INSTALL_DIR})
add_custom_target(_clean rm -rf ./CMakeCache.txt && rm -r CMakeFiles)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "**/*.c")
foreach(e IN LISTS SOURCES)
    foreach(pat IN LISTS APP_IGNORE_COMPILE)
        if(${e} MATCHES ${pat})
            message("remove compile target: ${e}")
            list(REMOVE_ITEM SOURCES ${e})
            break()
        endif()
    endforeach()
endforeach()

link_directories (
    ${GLIB_LIBRARY_DIRS}
    ${BONE_LIBRARY_DIRS}
)
add_library(${APP_EXECUTABLE} SHARED ${SOURCES})
add_definitions (
    ${GLIB_CFLAGS_OTHER}
)
target_include_directories(
    ${APP_EXECUTABLE}
    PRIVATE
    ${GLIB_INCLUDE_DIRS}
    ${BONE_INCLUDE}
)
target_link_libraries(
    ${APP_EXECUTABLE}
    PRIVATE
    ${GLIB_LIBRARIES}
    ${BONE_LIBRARIES}
    ${CMAKE_DL_LIBS}
    m
)
set_target_properties(varProjectName
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib"
    LIBRARY_OUTPUT_DIRECTORY "../lib"
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)
EOS
cmake_option = cmake_option.gsub("your bone header", Dir.pwd + "/src")
cmake_option = cmake_option.gsub("your bone library", Dir.pwd + "/lib")
src_cmake = Bone::unique_file(src_dir + "/CMakeLists.txt")
File.open(src_cmake, "w") do |fp|
    fp.write(cmake_option)
end

# create ../bone_embed/src/plugin/main.c
main = <<-EOS
#include <stdio.h>
#include <glib.h>
#include <bone/runtime/interpreter.h>


void varProjectName_Init(bnInterpreter* bone) {
    printf("hello Init");
}
void varProjectName_Destroy(bnInterpreter* bone) {
    printf("hello Destroy");
}
EOS
src_main = Bone::unique_file(src_app_dir + "/hello.c")
File.open(src_main, "w") do |fp|
    fp.write(main)
end
# create ../bone_embed/bin/bone.bn
bone = <<-EOS
{} <- include("file.bn");
stdout.puts("hello, world");
EOS
src_bone = Bone::unique_file(bin_dir + "/bone.bn")
File.open(src_bone, "w") do |fp|
    fp.write(bone)
end

Dir.open(Dir.pwd + "/bin") do|dirp|
    dirp.each do|file|
        next if file.start_with?(".")
        path = Dir.pwd + "/bin/" + file
        if(File.directory?(path))
            FileUtils.cp_r(path, project_dir + "/bin")
        else
            FileUtils.cp(path, project_dir + "/bin/" + File.basename(path))
        end
    end
end
FileUtils.cp("ffi.rb", src_app_dir + "/ffi.rb")

# make project
Dir.chdir(src_dir) do
    o, e, s = Open3.capture3("cmake .")
    puts o
    if s != 0
        puts e
    end
    o, e, s = Open3.capture3("make")
    puts o
    if s != 0
        puts e
    end
end