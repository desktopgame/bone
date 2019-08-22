# bone 仕様書

bone は動的型付けのプログラミング言語で、  
プロトタイピングの要素を持っています。

目玉となる機能はインジェクション演算子、名前つき戻り値です。

## コマンドライン引数

```
$ bone
```

インタラクティブモードで起動。  
ただし、デバッグビルドではテストを実行する。

```
$ bone filename args...
```

現在のパスから filename を解決して実行。  
args はスクリプト側から `argc` `argv` として参照できます。

## プロトタイピング

bone では存在しないメンバーに代入を行うと、  
その名前のメンバーを作成します。

bone ではメソッドという概念が存在しません。  
ただし、クロージャーは存在します。  
これをメンバーに代入することでメソッドの様に使えます。

## クロージャ(closure)

通常、クロージャーはインスタンスではなくスコープに依存するオブジェクトです。  
この点は他の多くの言語と同様です。

bone のクロージャが他のプログラム言語と異なるのは、  
"それを持っているオブジェクトに依存する"宣言ができる点です。

通常のクロージャ

```
//mdtest
stdio := object() <- load("file.bn");
FUNC := def (param, param2)() {
    stdio.stdout.puts(param);
    stdio.stdout.puts(param2);
};
FUNC("hoge", "huga");

obj := object();
obj.func := FUNC;
obj.func("hoge", "huga");
```

"それを持っているオブジェクトに依存する"クロージャ

```
//mdtest
stdio := object() <- load("file.bn");
FUNC := def (self, param)() {
    stdio.stdout.puts(self);
    stdio.stdout.puts(param);
};
FUNC("hoge", "huga");

obj := "self string";
obj.func := FUNC;
obj.func(/* ここにobj自身が入る*/ "huga");
```

この機能は演算子オーバーロードのために追加されました。

## クロージャから値を返す

クロージャから値を返すためには、名前つき戻り値を使用します。

名前つき戻り値

```
//mdtest
stdio := object() <- load("file.bn");
a := def() (value) {
    value := "aaa";

    // return value;
    // 値を返すreturnはboneでは使えません。

    return;
    // これは大丈夫です。
};
// "aaa" が val へ代入されます。
val := a();

// val へインジェクションを行います。
val <- a();
stdio.stdout.puts(val.value);

// 現在のスコープで "value" という名前の変数が定義されます。
{} <- a();
stdio.stdout.puts(value);

b := def() (return1, return2) {
    return1 := "value1";
    return2 := "value2";
};

// "value1"がvalに代入されます。
// 複数の名前つき戻り値を持つ関数が呼ばれた時、その最初の要素を返すからです。
// この仕様は、主要なデータを第一の名前つき戻り値にして、
// エラー情報やそのたオプショナルな情報を第二以降の名前つき戻り値にするといった使い方ができます。
// 上記の例では戻り値が一つだけでしたが、あれもこの仕様が適用された結果です。
// 名前つき戻り値が一つの時のみの特例ではありません。
val := b();

// val へインジェクションを行います。
val <- b();
stdio.stdout.puts(val.return1);
stdio.stdout.puts(val.return2);

// 現在のスコープで "return1", "return2" という名前の変数が定義されます。
{} <- b();
stdio.stdout.puts(return1);
stdio.stdout.puts(return2);
```

他の多くの言語と異なるのは、値を返すためには常に名前つき戻り値を使わなければいけない点です。

もし通常の値つき return が使えると仮定すると、
この仕様では曖昧な振る舞いが生じます。

```
a := def () (retA, retB) {
    retA := "aaa"
    retB := "bbb"
    return ???
}
```

## インジェクション

インジェクションは上で紹介した以下のコードです。

メンバーの定義、もしくはローカル変数の宣言ができます。  
ローカル変数の宣言は名前つき return を行うクロージャの中で有用です。

```
...

b := def() (return1, return2) {
    return1 := "value1"
    return2 := "value2"
};

...

// val へインジェクションを行います。
val <- b();
println(val.return1);
println(val.return2);

// 現在のスコープで "return1", "return2" という名前の変数が定義されます。
{} <- b();
println(return1);
println(return2);

...
```

ネストした名前つき戻り値のサンプル

```
//mdtest
stdio := object() <- load("file.bn");
a := def() (val, val2) {
    val := "aaa";
    val2 := "bbb";
};

b := def() (val, val2, val3) {
    {} <- a();
    val3 := "cccc";
};
{} <- b();
stdio.stdout.puts(val);
stdio.stdout.puts(val2);
stdio.stdout.puts(val3);
```

## 可変長名前つき戻り値

そのスコープで定義された全ての変数を返す場合は、
次の構文を使用します。

```
varret := def ()(...) {
    a := "aaa";
    b := "ccc";
    c := "ddd";
};
{} <- varret();
println(a);
println(b);
println(c);

// 可変長名前つき戻り値を直接格納する場合、
// 配列へラップされます。
// "最初の要素" が存在しないからです。
arr := varret();
println(arr[0]);
println(arr[1]);
println(arr[2]);
```

load は内部的にこれを使用しています。

## エクスポートされない変数

load や include などの可変長名前つき戻り値によって実装される関数は、  
公開されるべきでない変数を公開してしまう可能性があります。

```
temp := 0;

work := def()() {
    ...
    tempを使って何か処理をする
};
```

これは一貫性を欠きますが、以下の様な仕様を採用しました。

```
_temp := 0;
//これは公開されない

hoge := def()(_value) {
    _value := ...
};
//この様に数が決まっている戻り値では _ を使える。

{} <- hoge();
//ただし、トップレベルにエクスポートされると、
//それをload, includeするファイルからは見えない
```

## クロージャーのオーバーロード

できません。

## 組み込み変数

VALUE:

- true
- false
- argc
- argv

CLOSURE:

- include
- load
- object
- array
- string
- panic
- recover
- extern_var
- extern_def

詳細は[こちら](blt.md)

## 予約語

いくつかの名前は演算子オーバーロードのために予約されています。

```
opCall
opPositive
opNegative
opChilda
opNot
opPlus
opMinus
opMultiply
opDivide
opModulo
opBitAnd
opBitOr
opExcOr
opLeftShift
opRightShift
opGT
opGE
opLT
opLE
opEqual
opNotEqual
opArraySet
opArrayGet
```

演算子オーバーロードの使用例

```
a := 10;
a.opPlus := def(self, other)(ret) {
    ret := "hello";
};
println(a + 1);
//hello
```

## 真偽値型

bone には if 文, while 文が存在しており、
その仕様は C を踏襲しています。

if 文, while 文で "true" として扱われるのは

- false, 0 以外のオブジェクト全て

if 文, while 文で "false" として扱われるのは

- false
- 0

例えば、次のコードは完全に合法です。

```
count := 10;
while(count) {
    println(count.toString());
    count -= 1;
}
```

## 短絡評価

ruby の仕様に近いです。

最後に評価された値を返します。

```
a := "aaa" && "bbb";
//aは"bbb"

a := "aaa" || "bbb";
//aは"aaa"

a := false && "aaa";
//aはfalse
```

## null

ありません。  
代わりに存在しない要素が参照された時には panic します。

```
example A:
    a := object();
    println(a.key); // ここでpanic

exampleB:
    a := def()(x, y) {
        x := 10;
        // yにはデフォルトオブジェクトが入ります。
        // つまり次のコードと等価です。
        // y := object();
        return;
    }

exampleC:
    a := def()() {

    };
    b := a(); // ここでpanic
```

## C とのバインディング

bone から C の変数、関数を参照するには次の関数を使用します。

```
stdout := extern_var("stdout");
fopen := extern_def("fopen", ["path", "mode"], ["file", "exists"]);
```

extern_def の代わりに extern_var を使うこともできます。  
extern_def を使うことの利点は関数の引数,戻り値をソースコードで表明できる点です。

```
fopen := extern_var("fopen");
```

## 例外処理

bone では、二種類の例外処理の方法が存在します。  
一つは bone が複数の戻り値を返すことを活かしたものです。

戻り値が一つの場合

```
unsafe := def(index)(error) {
    if(index == 0) {
        error := "index is zero";
    }
};
error := unsafe(0);
if(error) {
    println(error);
}
```

戻り値が二つ以上の場合

```
unsafe := def(index)(value, error) {
    value := "not error";
    error := false;
    if(index == 0) {
        value := "error";
        error := "index is zero";
    }
};
a := object() <- unsafe(0);
if(a.error) {
    println(a.value);
}
```

戻り値が二つ以上の場合

```
unsafe := def(index)(value, error) {
    value := "not error";
    error := false;
    if(index == 0) {
        value := "error";
        error := "index is zero";
    }
};
{} <- unsafe(0);
if(error) {
    println(value);
}
```

二つ目はパニックを使ったものです。  
これは go 言語に習った仕様になっています。

```
p := def()() {
    defer println(recover());
    panic("recover");
};
p();
```

## エラーメッセージ

パニックによる終了の時

```
panic:error message
```

それ以外による終了の時

```
abort:error message
```

## 実装が保留されているもの

実装が難しいか、言語を汚くする可能性があるために保留されているもの。  
もしくは必要性が疑わしいもの。

### パターンマッチ

インジェクションする要素を選択する

```
windowから始まる名前で、かつscreenが含まれる名前だけをインジェクションする。

{} <- injection() match (elem) a && b {
    case a:
        return elem.startWith("window");
    case b:
        return elem.contains("screen");
}

{} <- injection() match (インジェクションされる名前をキャプチャする変数名) ケースを使用してtrueを返す式を記述 {
    case ケースの名前:
        bool を返す式
    case ケースの名前:
        bool を返す式
}
```

### パターンマッチ 2

よりシンプルなもの。

```
fullName, hogeのみをインジェクションする。

{} <- injection() match ["fullName", "hoge"]
```

### パターンマッチ 3

```
fun := def()(a, b, c, d, error) {
    ...
};
{a, b, error} := fun();
```

### 配列の展開

```
fun := def(a, b, c)() {
    ...
}
ary := array(3);
ary[0] := 1;
ary[1] := 2;
ary[2] := 3;
fun(ary...)
//オブジェクト... で、その中身を一段階展開する。
//C++のパラメータパック展開のようなもの
```

### デフォルト引数

```
fun := def(paramA, paramB)() {
    ...
};
fun.paramA := "default value";
fun.paramB := 10;

fun();
fun("myValue", 20);
```

### デフォルト戻り値

```
fun := def()(retA, retB) {
    ...
};
fun.retA := "default value";
fun.retB := 10;
```

### ドキュメンテーションコメント

```
fun := def()(retA, retB) {
    ...
};
fun.comment := "funはホゲホゲをフガフガします。";
```
