# bone 仕様書(bone language specification)
bone は動的型付けのプログラミング言語で、  
プロトタイピングの要素を持っています。  
(bone is dynamic typing language,  
have prototyping function)  
  
目玉となる機能はインジェクション演算子、名前つき戻り値です。  
(feature is injection operator)  

## プロトタイピング(prototyping)
boneでは存在しないメンバーに代入を行うと、  
その名前のメンバーを作成します。  
(create new member that name if assigned to not found member)  
  
boneではメソッドという概念が存在しません。  
ただし、クロージャーは存在します。  
これをメンバーに代入することでメソッドの様に使えます。  
(not exists method on `bone`.
however, exists closure object.
by doing assigned to member is useable like method)

## クロージャ(closure)
通常、クロージャーはインスタンスではなくスコープに依存するオブジェクトです。  
この点は他の多くの言語と同様です。  
(in normal, closure is not depend on instance,
closure is depend on call scope.
this point is like other programming launguages.)  
  
boneのクロージャが他のプログラム言語と異なるのは、  
"それを持っているオブジェクトに依存する"宣言ができる点です。  
(difference of bone closure between other language is
can declare of depend on assigned instance)

通常のクロージャ(normal closure)
````
FUNC := def (param, param2,,,) {
    ...
};
FUNC("hoge", "huga");

obj := ...
obj.func = FUNC;
obj.func("hoge", "huga");
````

"それを持っているオブジェクトに依存する"クロージャ(depend on assigned instance)
````
FUNC := def (self, param, param2,,,) {
    ...
};
FUNC("self", "hoge", "huga");

obj := ...
obj.func = FUNC;
obj.func(/* ここにobj(here is obj) */ "hoge", "huga");
````

この機能は演算子オーバーロードのために追加されました。  
(this function added for operator overload)

## クロージャから値を返す(return value from closure)
クロージャから値を返すためには、名前つき戻り値を使用します。  
(need named return for use return value from closure)

名前つき戻り値(named return)
````
a := def() (value) {
    value := "aaa"

    // return value;
    // 値を返すreturnはboneでは使えません。
    // can't use return with value

    return;
    // これは大丈夫です。
    // it's ok
};
// "aaa" が val へ代入されます。
// "aaa" assigned into val
val := a();

// val へインジェクションを行います。
// injection to val
val <- a();
println(val.value);

// 現在のスコープで "value" という名前の変数が定義されます。
// declared a value at current scope
{} <- a();
println(value)

b := def() (return1, return2) {
    return1 := "value1"
    return2 := "value2"
};

// "value1"がvalに代入されます。
// "value1" assigned into val
//
// 複数の名前つき戻り値を持つ関数が呼ばれた時、その最初の要素を返すからです。
// (return of first value when has multiple "named return")
// この仕様は、主要なデータを第一の名前つき戻り値にして、
// エラー情報やそのたオプショナルな情報を第二以降の名前つき戻り値にするといった使い方ができます。
// (this rule is can useable to return of optional data)
// 上記の例では戻り値が一つだけでしたが、あれもこの仕様が適用された結果です。
// 名前つき戻り値が一つの時のみの特例ではありません。
// (also when example on above "named return" count is one,
// it is the result of applyed this rule.
// NOT special specification)
val := b();

// val へインジェクションを行います。
// injection to val
val <- b();
println(val.return1);
println(val.return2);

// 現在のスコープで "return1", "return2" という名前の変数が定義されます。
// declared a value at current scope
{} <- b();
println(return1);
println(return2);
````

他の多くの言語と異なるのは、値を返すためには常に名前つき戻り値を使わなければいけない点です。  
(difference of bone between other language is
use always "named return" for return value from closure)  
  
もし通常の値つき return が使えると仮定すると、
この仕様では曖昧な振る舞いが生じます。  
(have fuzzy behavior if can use return with value)
````
a := def () (retA, retB) {
    retA := "aaa"
    retB := "bbb"
    return ???
}
````

# インジェクション(injection)
インジェクションは上で紹介した以下のコードです。  
  
メンバーの定義、もしくはローカル変数の宣言ができます。  
ローカル変数の宣言は名前つきreturnを行うクロージャの中で有用です。  
(can declare member or declare local variable.  
declare local variable by injection is convenient when implement named return)
````
...

b := def() (return1, return2) {
    return1 := "value1"
    return2 := "value2"
};

...

// val へインジェクションを行います。
// injection to val
val <- b();
println(val.return1);
println(val.return2);

// 現在のスコープで "return1", "return2" という名前の変数が定義されます。
// declared a value at current scope
{} <- b();
println(return1);
println(return2);

...
````

ネストした名前つき戻り値のサンプル(sample of nested named return)
````
a := def() (val, val2) {
    val := "aaa"
    val2 := "bbb"
};

b := def() (val, val2, val3) {
    {} <- a()
    val3 = "cccc"
}
````

# 可変長名前つき戻り値(translate is comming later)
そのスコープで定義された全ての変数を返す場合は、
次の構文を使用します。  
(translate is comming later)
````
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
// (translate is comming later)
arr := varret();
println(arr[0]);
println(arr[1]);
println(arr[2]);
````

loadは内部的にこれを使用しています。  
(translate is comming later)

# クロージャーのオーバーロード(overload of closure)
できません。
(can't)

# 組み込み変数(built-in variable)
VALUE:
* true
* false

CLOSURE:
* load
* object
* array
* extern_var
* extern_def

# 真偽値型(boolean type)
boneにはif文, while文が存在しており、
その仕様はCを踏襲しています。  
(bone have if and while statement.
it specification is like C)  
  
if文, while文で "true" として扱われるのは
* true
* 0以外の数字
* false, 0以外のオブジェクト全て

if文, while文で "false" として扱われるのは
* false
* 0

例えば、次のコードは完全に合法です。  
(in example, next code is just completly no problem)
````
count := 10;
while(count) {
    println(count.toString());
    count -= 1;
}
````

# null
ありません。(bone is'nt have a null)  
代わりに存在しない要素が参照された時にはpanicします。  
````
example A:
    a := object();
    println(a.key); // ここでpanic(panic on here)

exampleB:
    a := def()(x, y) {
        x := 10;
        // yにはデフォルトオブジェクトが入ります。
        // つまり次のコードと等価です。
        // (y is default object.
        // equality to next code.)
        // y := object();
        return;
    }

exampleC:
    a := def()() {

    };
    b := a(); // ここでpanic(panic on here)
````

# Cとのバインディング(binding with c)
boneからCの変数、関数を参照するには次の関数を使用します。  
(use next function for reference to function in C from bone)
````
stdout := extern_var("stdout");
fopen := extern_def("fopen", ["path", "mode"], ["file", "exists"]);
````

extern_defの代わりにextern_varを使うこともできます。  
extern_defを使うことの利点は関数の引数,戻り値をソースコードで表明できる点です。
````
fopen := extern_var("fopen");
````

# 例外処理(exception handling)
boneでは、二種類の例外処理の方法が存在します。  
一つはboneが複数の戻り値を返すことを活かしたものです。

戻り値が一つの場合(when in number of return value one)
````
unsafe := def(index)(error) {
    if(index == 0) {
        error := "index is zero";
    }
};
error := unsafe(0);
if(error) {
    println(error);
}
````

戻り値が二つ以上の場合(when in number of return value greater than equal one)
````
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
````
戻り値が二つ以上の場合(when in number of return value greater than equal one)
````
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
````

二つ目はパニックを使ったものです。  
これはgo言語に習った仕様になっています。  
(the second is by panic.
it is like a go language)
````
p := def()() {
    defer println(recover());
    panic("recover");
};
p();
````

# 実装が保留されているもの(pending function)
実装が難しいか、言語を汚くする可能性があるために保留されているもの。  
もしくは必要性が疑わしいもの。  
(translate is comming later)

## パターンマッチ(pattern match)
インジェクションする要素を選択する  
(translate is comming later)
````
windowから始まる名前で、かつscreenが含まれる名前だけをインジェクションする。
(translate is comming later)

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
````

## パターンマッチ2(pattern match2)
よりシンプルなもの。
(translate is comming later)

````
fullName, hogeのみをインジェクションする。
(translate is comming later)

{} <- injection() match ["fullName", "hoge"]
````

## エクスポートされない変数(not export variable)
現状の仕様では、include, load は全ての変数を公開します。  
(in now, include and load function is export all variable)  

### 解決策1(solution 1)
goに習って例えば大文字から始まる変数はエクスポートしない、などの仕様を考案中。  
(translate is comming later)  
* 一貫性を重視するならば、名前つき戻り値も大文字から始めなければいけない？
  * 可変長名前つき戻り値のことを考えると、一応意味はある。
  * 小文字の名前つき戻り値が使用されていたらコンパイルエラー？
    * 例外ではない
    * strictみたいな感じで必ず検査される
### 解決策2(solution 2)
予約語`local`の導入。  
````
local a := ...
````
* ダサいが無難
* 他の言語にも似た様なやつがある
  * perlなど
    * my にすれば多少タイプ数はマシになる
* タイプがめんどくさい
* 内部的に宣言と代入を分ける必要がありそう
  * 実装がめんどくさい

### 解決策3(solution 3)
変数テーブルにアクセスする手段を提供する。  
変数テーブルをboneのハッシュオブジェクトとして参照可能にする。  
* 多分前例がない
* 面白そうではある

### 解決策4(solution 4)
解決策を提供しない。
* 問題が完全に把握できないときは、解決策も提供しないのが最善の方法である。(X Window 基本原則より)