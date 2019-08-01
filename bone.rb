require 'rbconfig'

module Bone
    def self.check_cwd
        entries = []
        Dir.open(".") do|dirp|
            dirp.each do |file|
                entries << file
            end
        end
        message = sprintf("%s is not bone repository", Dir.pwd)
        raise message if !entries.include?("src")
        raise message if !entries.include?("doc")
        raise message if !entries.include?("lib")
        raise message if !entries.include?("bin")
        raise message if !entries.include?("template")
        raise message if !entries.include?("setup_sln.rb")
        Dir.pwd
    end

    def self.unique_dir(dir)
        basename = dir
        count = 1
        while Dir.exists?(dir)
            dir = basename + "(" + count.to_s + ")"
            count += 1
        end
        dir
    end

    def self.os
      @os ||= (
        host_os = RbConfig::CONFIG['host_os']
        case host_os
        when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
          :windows
        when /darwin|mac os/
          :macosx
        when /linux/
          :linux
        when /solaris|bsd/
          :unix
        else
          :unknown
        end
      )
    end
end