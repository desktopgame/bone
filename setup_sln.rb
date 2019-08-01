require "open3"
require 'open-uri'
require 'Fileutils'
require_relative "bone"

def select_dll_base(base_dir)
    files = []
    Dir.open(base_dir) do|dirp|
        dirp.each do|file|
            next if file == "." || file == ".."
            path = (base_dir + "\\" + file)
            if File.directory?(path)
                select_dll_base(path).each do |e|
                    files << e
                end
            else
                files << path
            end
        end
    end
    files
end

def select_dll(solution_dir)
    pack_dir = solution_dir + "\\packages"
    dlls = select_dll_base(pack_dir)
    dlls
end

def copy_dll(solution_dir)
    dlls = select_dll(solution_dir)
    dlls = dlls.filter {|e| e.include?(".dll")}
    glib_dlls = dlls.filter{|e| e.include?("glib.redist")}
    glib_dlls = glib_dlls.filter{|e| e.include?("Win32")}
    glib_dlls = glib_dlls.filter{|e| e.include?("v110")}
    glib_dlls = glib_dlls.filter{|e| e.include?("Debug")}
    libintl_dlls = dlls.filter{|e| e.include?("libintl.redist")}
    libintl_dlls = libintl_dlls.filter{|e| e.include?("Win32")}
    libintl_dlls = libintl_dlls.filter{|e| e.include?("v110")}

    glib_dlls.each do |dll|
        output = solution_dir + "\\" + File.basename(dll)
        FileUtils.cp(dll, output)
    end
    libintl_dlls.each do |dll|
        output = solution_dir + "\\" + File.basename(dll)
        FileUtils.cp(dll, output)
    end
end

Bone::check_cwd()
# create solution directory
solution_dir = Bone::unique_dir(File::dirname(Dir::pwd) + "\\bone_sln")
Dir.mkdir(solution_dir)
# create project
puts("create project...")
o, e, s = Open3.capture3(sprintf("cmake -D BONE_RUN=1 -S %s\\src -B %s", Dir::pwd, solution_dir))
if s != 0
    puts o
    puts e
end
# download header
puts("download header...")
GLIB_CONFIG_URL = "https://gist.githubusercontent.com/tversteeg/ba848fad54a96754b64a316ca91b4968/raw/5b4e66b7561e092ebcfe4f3fcef478550b5e4ecc/glibconfig.h"
Dir.mkdir(solution_dir + "\\include")
open(GLIB_CONFIG_URL) do |request|
    File.open(solution_dir + "\\include\\glibconfig.h", "w") do |file|
        file.write(request.read)
    end
end
# download script
puts("download script...")
VS_PATCH_URL = "https://gist.githubusercontent.com/desktopgame/8083490657442cbde305d615399346a4/raw/b0b9c376f6eb1bc2491d70cefa8463be2792d849/vs_patch.rb"
open(VS_PATCH_URL) do |request|
    File.open(solution_dir + "\\vs_patch.rb", "w") do |file|
        file.write(request.read)
    end
end
# run script
puts("run script...")
Dir.chdir(solution_dir) do
    o, e, s = Open3.capture3("ruby vs_patch.rb")
    if s != 0
        puts o
        puts e
    end
end
# download nuget
puts("download nuget...")
NUGET_URL = "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
Dir.mkdir(solution_dir + "\\packages")
open(NUGET_URL) do |request|
    File.open(solution_dir + "\\packages\\nuget.exe", "wb") do |file|
        file.write(request.read)
    end
end
# download glib
puts("download glib...")
Dir.chdir(solution_dir + "\\packages") do
    o, e, s = Open3.capture3("nuget install glib")
    if s != 0
        puts o
        puts e
    end
end
copy_dll(solution_dir)
Dir.open(Dir.pwd + "\\bin") do|dirp|
    dirp.each do|file|
        next if file.start_with?(".")
        path = Dir.pwd + "\\bin\\" + file
        if(File.directory?(path))
            FileUtils.cp_r(path, solution_dir)
        else
            FileUtils.cp(path, solution_dir + "\\" + File.basename(path))
        end
    end
end
puts("created project successfully")
puts(solution_dir)