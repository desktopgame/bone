require "open3"
require "fileutils"
require "date"

def clean()
    FileUtils.rm_r("CMakeFiles") if(File.exists?("CMakeFiles"))
    File.delete("CMakeCache.txt") if(File.exists?("CMakeCache.txt"))
end

def build(type)
    o, e, s = Open3.capture3("cmake . -D" + type + "=1")
    o, e, s = Open3.capture3("make")
end

clean()
build("BONE_SHARED")
clean()
build("BONE_STATIC")
clean()
build("BONE_RUN")