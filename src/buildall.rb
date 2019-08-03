require "open3"
require "fileutils"
require "date"

# check args
if ARGV.length == 0
    puts('please build option')
    puts('example: ruby buildall.rb Debug')
    puts('example: ruby buildall.rb Release')
    abort
end
NAME = ARGV[0]

def clean()
    FileUtils.rm_r("CMakeFiles") if(File.exists?("CMakeFiles"))
    File.delete("CMakeCache.txt") if(File.exists?("CMakeCache.txt"))
end

def build(mode, type)
    cmd = "cmake . -DCMAKE_BUILD_TYPE=" + mode + " -D" + type + "=1";
    puts cmd
    o, e, s = Open3.capture3(cmd)
    puts e
    o, e, s = Open3.capture3("make")
    puts e
end

def main(option)
    clean()
    build(option, "BONE_SHARED")
    clean()
    build(option, "BONE_STATIC")
    clean()
    build(option, "BONE_RUN")
    FileUtils.cp("../lib/libbone.dylib", "../bin/libbone.dylib")
end
main(ARGV[0])