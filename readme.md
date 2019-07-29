# 概要(about bone language)
![](https://travis-ci.org/desktopgame/bone.svg?branch=master)

boneはプロトタイピング言語をベースとしており、  
いくつか独自のアイデアも加えられています。  
また、この言語は実験的なものなので  
他の言語にあるような人気のある機能のいくつかは含まれません。  
(bone is based on prototyping language.  
and, added some peculiar idea.  
also, this language is but experiment,
so, included in other languages popular function is not included in this language.)  

例(example):
* class, interface, overload
* method
* exception,try-catch
* null

# 参考にした言語(affected a language)
* [crowbar](http://kmaebashi.com/programmer/devlang/crowbar.html)
* [anko](https://github.com/mattn/anko)
* [go](https://github.com/golang/go)
* [ruby](https://github.com/ruby/ruby)

# 特徴(feature)
この言語独自のアイデアはオブジェクトインジェクションとスコープインジェクションです。  
これは名前つき戻り値と関連しています。  
また、値を返す時は常に名前つき戻り値を使います。  
(this language peculiar idea is object injection, and scope injection.  
this idea is related with named return value.  
and, need always nnamed return value if return a value.)

## 具体例(concrete example)
名前つき戻り値(named return value)
````
f := def()(value, error) {
    value := "value is string";
    error := false;
};

// オブジェクトインジェクション(object injection)
v := object() <- f();
println(v.value);
println(v.error);

// スコープインジェクション(scope injection)
{} <- f();
println(value);
println(error);

// ネストした名前つき戻り値(nested named return value)
f2 := def()(value, error, value2) {
    {}  <- f();
    value2 := "value 2";
};
````
オブジェクトインジェクション(object injection)
````
std := object() <- load("./std.bn");
std.printf("hello, world");
````
スコープインジェクション(scope injection)
````
{} <- load("./std.bn");
printf("hello, world");
````

# 詳細(more information)
詳細はdocumentフォルダを参照してください。  
(visit document folder if read more information)