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

## クロージャとオペレータ(closure and operator)
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

// 現在のスコープで "value" という名前の変数が定義されます。
// declared a value at current scope
{} <- a();
println(value)

b := def() (return1, return2) {
    return1 := "value1"
    return2 := "value2"
};

// ハッシュがvalに代入されます。
// hash assigned into val
val := b();
println(val.get("return1"));
println(val.get("return2"));

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

# 例外処理(exception handling)
現在boneには例外処理のための特別な構文が存在しません。  
(not exists exception handling syntax)  
  
Goに習った例外処理であれば今すぐにでも可能ですが、
ライブラリレベルのサポートはまだありません。  
(Right now can exception handling like Go.  
however, not provide package for error like go)  
  
````
a := def readFile() (txt, error) {
    ...

    error := false;
    if( not opened file) {
        error := true
        return;
    }
};
val <- a()
if(val.error) {
    ...
}
````