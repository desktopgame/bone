require 'fileutils'
Dir.glob("./suc/*.in") do |file|
    path = file + ".out"
    to = path
    to = to.gsub("suc", "cmp");
    to = to.gsub(".out", ".cmp");
    if(File.exists?(to)) then
        File.delete(to)
    end
    FileUtils.cp(path, to)
    puts (path + " -> " + to)
end