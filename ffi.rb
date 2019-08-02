require 'fileutils'
require 'pp'

DOT_FFI = '.ffi'
H_FFI = 'ffi.h'
C_FFI = 'ffi.c'
NAME = ARGV[0]

class Parameter
    attr_accessor :type, :name

    def initialize(type, name)
        @type = type
        @name = name
    end
end

class Function
    attr_accessor :name, :return_type, :parameter_list

    def initialize(name, return_type)
        @name = name
        @return_type = return_type
        @parameter_list = []
    end
end

unless File.exist?(DOT_FFI)
  File.open(DOT_FFI, 'w') do |fp|
    fp.puts('// this file is for ffi')
    fp.puts('// define a export function by next syntax')
    fp.puts('%%BEGIN')
    fp.puts('static double my_sin(double x) {')
    fp.puts('        printf("my_sin¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static double my_cos(double x) {')
    fp.puts('        printf("my_cos¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static double my_tan(double x) {')
    fp.puts('        printf("my_tan¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static double my_max(double x) {')
    fp.puts('        printf("my_max¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static double my_min(double x) {')
    fp.puts('        printf("my_min¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static void my_exit(int status) {')
    fp.puts('        printf("my_exit¥n");')
    fp.puts('}')
    fp.puts('%%END')
    fp.puts('$function/double/my_sin(double x);')
    fp.puts('$function/double/my_cos(double x);')
    fp.puts('$function/double/my_tan(double x);')
    fp.puts('$function/double/my_max(double x, double y);')
    fp.puts('$function/double/my_min(double x, double y);')
    fp.puts('$function/void/my_exit(int status);')
  end
  puts('was created  `.ffi`')
  puts('please agein execute when after edit `.ffi`')
  exit(0)
end
if ARGV.length == 0
    puts('please ffi name')
    puts('example: ruby ffi.rb myFFI')
end
BONE_FFI = sprintf('ffi_%s.bn', NAME)
functions = []
inline_lines = []
in_inline = false
File.open(DOT_FFI) do |fp|
  text = fp.read
  text.lines do |line|
    next if line.start_with?('//')
    line = line.rstrip
    if line == '%%BEGIN'
      in_inline = true
      next
    end
    if in_inline
      if line == '%%END'
        in_inline = false
      else
        inline_lines << line
      end
      next
    end
    words = line.split('/')
    if words[0] == '$function'
      ret_type = words[1]
      body = words[2]
      name = body[0, body.index('(')]
      f = Function.new(name, ret_type)
      # read params
      params = body[body.index('(') + 1, body.index(')') - (body.index('(') + 1)]
      params.split(',').each do|pair|
        param = pair.split(' ')
        f.parameter_list << Parameter.new(param[0], param[1])
      end
      functions << f
    end
  end
end
# create .h
if File.exist?(H_FFI)
    File.delete(H_FFI)
end
File.open(H_FFI, "w") do |fp|
    fp.puts('#ifndef FFI_H')
    fp.puts('#define FFI_H')
    fp.puts('#include <bone/runtime/interpreter.h>')
    fp.puts('void ffi_init(bnInterpreter* bone);')
    fp.puts('void ffi_destroy(bnInterpreter* bone);')
    fp.puts('#endif')
end
# create .c
if File.exist?(C_FFI)
    File.delete(C_FFI)
end
File.open(C_FFI, 'w') do |fp|
  fp.puts('#include "ffi.h"')
  fp.puts('#include <stdio.h>')
  fp.puts('#include <bone/runtime/interpreter.h>')
  fp.puts('#include <bone/runtime/frame.h>')
  fp.puts('#include <bone/runtime/lambda.h>')
  fp.puts('#include <bone/runtime/object.h>')
  fp.puts('#include <bone/runtime/string.h>')
  fp.puts('#include <bone/runtime/integer.h>')
  fp.puts('#include <bone/runtime/char.h>')
  fp.puts('#include <bone/runtime/double.h>')
  fp.puts('')
  inline_lines.each do |inline_line|
    fp.puts(inline_line)
  end
  functions.each do |f|
    fp.puts(sprintf('static void ffi_%s_%s(bnInterpreter* bone, , bnFrame* frame) {', NAME, f.name))
    f.parameter_list.each_with_index do |param, i|
        fp.puts(sprintf('        // parameter[%d] %s', i, param.name))
        fp.puts(sprintf('        bnObject* arg%d = bnPopStack(frame->vStack);', i))
        if param.type == 'int'
            fp.puts(sprintf('        if(arg%d->type != BN_OBJECT_INTEGER) {', i))
            fp.puts(sprintf('                bnFormatThrow(bone, "`%s` is shoud be int");', param.name))
            fp.puts('        }')
            fp.puts(sprintf('        int val%d = ((bnInteger*)arg%d)->value;', i, i))
        elsif param.type == 'double'
            fp.puts(sprintf('        if(arg%d->type != BN_OBJECT_DOUBLE) {', i))
            fp.puts(sprintf('                bnFormatThrow(bone, "`%s` is shoud be double");', param.name))
            fp.puts('        }')
            fp.puts(sprintf('        double val%d = ((bnDouble*)arg%d)->value;', i, i))
        elsif param.type == 'char'
            fp.puts(sprintf('        if(arg%d->type != BN_OBJECT_CHAR) {', i))
            fp.puts(sprintf('                bnFormatThrow(bone, "`%s` is shoud be char");', param.name))
            fp.puts('        }')
            fp.puts(sprintf('        char val%d = ((bnChar*)arg%d)->value;', i, i))
        end
    end
    if f.return_type != 'void'
      fp.write(sprintf('        %s c_ret = ', f.return_type))
    end
    fp.write(sprintf('%s(', f.name))
    f.parameter_list.each_with_index do |param, i|
      if i == f.parameter_list.length - 1
        fp.write(sprintf('val%d);', i))
      else
        fp.write(sprintf('val%d, ', i))
      end
    end
    fp.puts('')
    fp.write('        ')
    if f.return_type == 'int'
        fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewInteger(bone, c_ret));'))
    elsif f.return_type == 'char'
        fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewChar(bone, c_ret));'))
    elsif f.return_type == 'double'
        fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewDouble(bone, c_ret));'))
    end
    fp.puts('')
    fp.puts('}')
    fp.puts('')
  end
  fp.puts('void ffi_init(bnInterpreter* bone) {')
  functions.each do |f|
    buf = '        g_hash_table_replace(bone->externTable,'
    buf += sprintf('bnIntern(bone->pool, "ffi.%s_%s"),', NAME, f.name)
    buf += sprintf('bnNewLambdaFromCFunc(bone, ffi_%s_%s, bone->pool,', NAME, f.name)
    if f.return_type != 'void'
        buf += 'BN_C_ADD_RETURN, "ret"'
    end
    if f.parameter_list.length > 0
        buf += ','
    end
    f.parameter_list.each do |param|
        buf += sprintf('BN_C_ADD_PARAM, "%s",', param.name)
    end
    buf += 'BN_C_ADD_EXIT);'
    fp.puts(buf)
  end
  fp.puts('}')
  fp.puts('')
  fp.puts('void ffi_destroy(bnInterpreter* bone) {')
  fp.puts('}')
end
# create .bn
if File.exist?(BONE_FFI)
    File.delete(BONE_FFI)
end
File.open(BONE_FFI, 'w') do |fp|
    functions.each do |f|
        fp.write(sprintf('%s := extern_def("ffi.%s_%s",', f.name, NAME, f.name))
        fp.write('[')
        f.parameter_list.each_with_index do|param, i|
            if i == f.parameter_list.length - 1
                fp.write(sprintf('"%s"', param.name))
            else
              fp.write(sprintf('"%s, "', param.name))
            end
        end
        fp.write('],')
        fp.write('["ret"]')
        fp.write(');')
        fp.puts('')
    end
end