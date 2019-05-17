require "open3"
require "fileutils"
require "date"

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

clean()
build("Release", "BONE_SHARED")
clean()
build("Release", "BONE_STATIC")
clean()
build("Release", "BONE_RUN")