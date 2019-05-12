require 'fileutils'
Dir.glob("./suc/*.in") do |file|
    path = file + ".out"
    to = path
    to = to.gsub("suc", "cmp");
    to = to.gsub(".out", ".cmp");
    pathBody = File.open(path, "r") {|fp| fp.read() }
    toBody = File.open(to, "r") {|fp| fp.read() }
    if(pathBody != toBody) then
        puts("failed test: " + file)
    end
end