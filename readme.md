[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6561b890c2ab43d3a6eef9e77f23d39a)](https://app.codacy.com/app/desktopgame/bone?utm_source=github.com&utm_medium=referral&utm_content=desktopgame/bone&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.org/desktopgame/bone.svg?branch=master)](https://travis-ci.org/desktopgame/bone)

# 概要

bone はプロトタイピング言語をベースとしており、  
いくつか独自のアイデアも加えられています。  
また、この言語は実験的なものなので  
他の言語にあるような人気のある機能のいくつかは含まれません。  
例:

- class, interface, overload
- method
- exception,try-catch
- null

# 参考にした言語

- [crowbar](http://kmaebashi.com/programmer/devlang/crowbar.html)
- [anko](https://github.com/mattn/anko)
- [go](https://github.com/golang/go)
- [ruby](https://github.com/ruby/ruby)

# 特徴

この言語独自のアイデアはオブジェクトインジェクションとスコープインジェクションです。  
これは名前つき戻り値と関連しています。  
また、値を返す時は常に名前つき戻り値を使います。

## 具体例

名前つき戻り値

```
f := def()(value, error) {
    value := "value is string";
    error := false;
};

// オブジェクトインジェクション
v := object() <- f();
println(v.value);
println(v.error);

// スコープインジェクション
{} <- f();
println(value);
println(error);

// ネストした名前つき戻り値
f2 := def()(value, error, value2) {
    {}  <- f();
    value2 := "value 2";
};
```

オブジェクトインジェクション

```
std := object() <- load("./std.bn");
std.printf("hello, world");
```

スコープインジェクション

```
{} <- load("./std.bn");
printf("hello, world");
```

# 詳細

詳細は document フォルダを参照してください。
