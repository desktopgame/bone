# encoding: utf-8
require "open3"
require "fileutils"
require "date"

OUTPUT = ".collect.out"

if File.exist?(OUTPUT)
    File.delete(OUTPUT)
end

def letter?(lookAhead)
    lookAhead =~ /[[:alpha:]]/
  end

def numeric?(lookAhead)
    lookAhead =~ /[[:digit:]]/
end

def isWord?(str)
    ret = true
    str.each_char do |c|
        if !letter?(c) && !numeric?(c) && c != '(' && c != ',' && c != '_'
            ret = false
        end
    end
    ret
end

def scanWord(str)
    if str.end_with?(",")
        return str[0, str.length - 1]
    end
    pos = str.index("(")
    if pos != nil
        return str[0, str.length - pos - 1]
    end
    return str
end

words = []
Dir.glob(File.expand_path('./../src/**/*')) do | filename |
    File.open(filename, "r") do |fp|
        next if File.directory?(filename)
        next if !filename.end_with?(".c") && !filename.end_with?(".h")
        next if filename.include?(".tab")
        next if filename.include?(".yy")
        txt = fp.read()
        txt.lines do |line|
            line = line.encode('UTF-8', :invalid => :replace).lstrip.rstrip
            blocks = line.split(" ")
            blocks.each do |block|
                if isWord?(block)
                    words << scanWord(block)
                end
            end
        end
    end
end
words = words.uniq
words.delete("return")
words.delete("switch")
words.delete("case")
words.delete("if")
words.delete("for")
words.delete("while")
words.delete("void")
words.delete("int")
words.delete("double")
words.delete("bool")
words.delete_if {|e| e.start_with?("0") }
words.delete_if {|e| e.start_with?("1") }
words.delete_if {|e| e.start_with?("_") }
words.delete_if {|e| e.start_with?("COMPILER_") }
words.delete_if {|e| e.start_with?("SIMULATE_") }
words.sort!()

File.open(OUTPUT, "w") do |fp|
    words.each do|word|
        fp.puts word
    end
end
