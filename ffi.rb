require 'fileutils'
require 'pp'

DOT_FFI = '.ffi'
H_FFI = 'ffi.h'
C_FFI = 'ffi.c'

class Parameter
    attr_accessor :type, :name, :unknown

    def initialize(type, name)
        @type = type
        @name = name
        @unknown = false
    end
end

class Type
  attr_accessor :name

  def initialize(name)
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
    fp.puts('static double my_max(double x, double y) {')
    fp.puts('        printf("my_max¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static double my_min(double x, double y) {')
    fp.puts('        printf("my_min¥n");')
    fp.puts('        return 0;')
    fp.puts('}')
    fp.puts('static void my_exit(int status) {')
    fp.puts('        printf("my_exit¥n");')
    fp.puts('}')
    fp.puts('static bool my_assert(bool cond) {')
    fp.puts('        printf("my_assert");')
    fp.puts('        return cond;')
    fp.puts('}')
    fp.puts('static void my_puts(char* str) {')
    fp.puts('        printf("my_puts");')
    fp.puts('}')
    fp.puts('static char* my_gets() {')
    fp.puts('        printf("my_puts");')
    fp.puts('        return "my_puts";')
    fp.puts('}')
    fp.puts('static char* my_cat(char* str) {')
    fp.puts('        return str;')
    fp.puts('}')
    fp.puts('%%END')
    fp.puts('$type/MyHoge')
    fp.puts('$function/double/my_sin(double x);')
    fp.puts('$function/double/my_cos(double x);')
    fp.puts('$function/double/my_tan(double x);')
    fp.puts('$function/double/my_max(double x, double y);')
    fp.puts('$function/double/my_min(double x, double y);')
    fp.puts('$function/void/my_exit(int status);')
    fp.puts('$function/bool/my_assert(bool cond);')
    fp.puts('$function/void/my_puts(char* str);')
    fp.puts('$function/char*/my_gets();')
    fp.puts('$function/char*/my_cat(char* str);')
    fp.puts('$function/char*/my_orig(MyHoge str);')
    fp.puts('$function/void*/my_generic(void* obj);')
  end
  puts('was created  `.ffi`')
  puts('please agein execute when after edit `.ffi`')
  exit(0)
end
if ARGV.length == 0
    puts('please ffi name')
    puts('example: ruby ffi.rb myFFI')
    abort
end
NAME = ARGV[0]
BONE_FFI = sprintf('ffi_%s.bn', NAME)
BONE_TEST = sprintf('ffi_%s_test.bn', NAME)
functions = []
types = []
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
    elsif words[0] == '$type'
      types << Type.new(words[1])
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
    fp.puts(sprintf('void ffi_%s_init(bnInterpreter* bone);', NAME))
    fp.puts(sprintf('void ffi_%s_destroy(bnInterpreter* bone);', NAME))
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
  fp.puts('#include <bone/runtime/any.h>')
  fp.puts('#include <bone/util/string_pool.h>')
  fp.puts('#include <glib.h>')
  fp.puts('')
  fp.puts('//')
  fp.puts('// Type')
  fp.puts('//')
  types.each do |type|
    tyname = type.name
    fp.puts(sprintf('typedef struct %s {', tyname))
    fp.puts('        bnAny base;');
    fp.puts(sprintf('} %s;', tyname))
    fp.puts(sprintf('static %s* bnNew%s(bnInterpreter* bone) {', tyname, tyname))
    fp.puts(sprintf('        %s* ret = BN_MALLOC(sizeof(%s));', tyname, tyname))
    fp.puts(sprintf('        bnInitAny(bone, &ret->base, "ffi.%s_%s");', NAME, tyname))
    fp.puts('        return ret;')
    fp.puts('}')
  end
  fp.puts('//')
  fp.puts('// Inline')
  fp.puts('//')
  inline_lines.each do |inline_line|
    fp.puts(inline_line)
  end
  fp.puts('//')
  fp.puts('// Function')
  fp.puts('//')
  functions.each do |f|
    fp.puts(sprintf('static void ffi_%s_%s(bnInterpreter* bone, bnFrame* frame) {', NAME, f.name))
    f.parameter_list.each_with_index do |param, i|
        fp.puts(sprintf('        // parameter[%d] %s', i, param.name))
        if param.type == 'int'
            fp.puts(sprintf('        bnPopIntArg(bone, frame, %s);', param.name))
            fp.puts(sprintf('        int %sVal = bnGetIntegerValue(%sObj);', param.name, param.name))
        elsif param.type == 'double'
            fp.puts(sprintf('        bnPopDoubleArg(bone, frame, %s);', param.name))
            fp.puts(sprintf('        double %sVal = bnGetDoubleValue(%sObj);', param.name, param.name))
        elsif param.type == 'char'
            fp.puts(sprintf('        bnPopCharArg(bone, frame, %s);', param.name))
            fp.puts(sprintf('        char %sVal = bnGetCharValue(%sObj);', param.name, param.name))
        elsif param.type == 'char*'
          fp.puts(sprintf('        bnPopStringArg(bone, frame, %s);', param.name))
          fp.puts(sprintf('        bnStringView %sVal = bnGetStringValue(%sObj);', param.name, param.name))
          fp.puts(sprintf('        const char* %sStr = bnView2Str(bone->pool, %sVal);', param.name, param.name))
        elsif param.type == 'bool'
          fp.puts(sprintf('        bnPopBoolArg(bone, frame, %s);', param.name))
          fp.puts(sprintf('        bool %sVal = bnGetBoolValue(%sObj);', param.name, param.name))
        elsif param.type == 'void*'
          fp.puts(sprintf('        bnPopArg(bone, frame, %s);', param.name))
        else
          param.unknown = true
          fp.puts(sprintf('        bnPopAnyArg(bone, frame, %s, "ffi.%s_%s");', param.name, NAME, param.type))
        end
    end
    if f.return_type == 'void*'
      fp.write(sprintf('        bnObject* c_ret = '))
    elsif f.return_type != 'void'
      fp.write(sprintf('        %s c_ret = ', f.return_type))
    end
    fp.write(sprintf('%s(', f.name))
    f.parameter_list.each_with_index do |param, i|
      if param.type == 'void*' || param.unknown
        argname = 'Obj'
      else
        argname = param.type == 'char*' ? 'Str' : 'Val'
      end
      if i == f.parameter_list.length - 1
        fp.write(sprintf('%s%s);', param.name, argname))
      else
        fp.write(sprintf('%s%s, ', param.name,argname))
      end
    end
    if f.parameter_list.size == 0
      fp.write(');')
    end
    fp.puts('')
    fp.write('        ')
    if f.return_type == 'int'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewInteger(bone, c_ret));'))
    elsif f.return_type == 'double'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewDouble(bone, c_ret));'))
    elsif f.return_type == 'char'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewChar(bone, c_ret));'))
    elsif f.return_type == 'char*'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnNewString2(bone, c_ret));'))
    elsif f.return_type == 'bool'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),bnGetBool(bone->pool, frame, c_ret));'))
    elsif f.return_type == 'void*'
      fp.puts(sprintf('g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),c_ret);'))
    end
    fp.puts('')
    fp.puts('}')
    fp.puts('')
  end
  fp.puts(sprintf('void ffi_%s_init(bnInterpreter* bone) {', NAME))
  functions.each do |f|
    buf = '        g_hash_table_replace(bone->externTable,'
    buf += sprintf('bnIntern(bone->pool, "ffi.%s_%s"),', NAME, f.name)
    buf += sprintf('bnNewLambdaFromCFunc(bone, ffi_%s_%s, bone->pool', NAME, f.name)
    if f.return_type != 'void'
        buf += ', BN_C_ADD_RETURN, "ret"'
    end
    buf += ','
    f.parameter_list.each do |param|
        buf += sprintf('BN_C_ADD_PARAM, "%s",', param.name)
    end
    buf += 'BN_C_ADD_EXIT));'
    fp.puts(buf)
  end
  fp.puts('}')
  fp.puts('')
  fp.puts(sprintf('void ffi_%s_destroy(bnInterpreter* bone) {', NAME))
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
              fp.write(sprintf('"%s", ', param.name))
            end
        end
        fp.write('],')
        if f.return_type == 'void'
          fp.write('[]')
        else
          fp.write('["ret"]')
        end
        fp.write(');')
        fp.puts('')
    end
end
# create .bn
if File.exist?(BONE_TEST)
    File.delete(BONE_TEST)
end
File.open(BONE_TEST, "w") do|fp|
  fp.puts(sprintf('{} <- load("ffi_%s.bn")', NAME))
  fp.puts('{} <- load("file.bn")')
end