require "open3"
require "fileutils"
require "date"

INPUT = ".prefix.in"
OUTPUT = ".prefix.out"

if !File.exist?(INPUT)
    FileUtils.touch(INPUT)
    return
end
if File.exist?(OUTPUT)
    File.delete(OUTPUT)
end

data = File.open(INPUT, "r") {|fp|  fp.read() }
File.open(OUTPUT, "w") do |fp|
    data.lines do |line|
        fp.puts("bn" + line)
    end
end