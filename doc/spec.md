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
}
FUNC("hoge", "huga")

obj := ...
obj.func = FUNC
obj.func("hoge", "huga")
````

"それを持っているオブジェクトに依存する"クロージャ(depend on assigned instance)
````
FUNC := def (self, param, param2,,,) {
    ...
}
FUNC("self", "hoge", "huga")

obj := ...
obj.func = FUNC
obj.func(/* ここにobj(here is obj) */ "hoge", "huga")
````

この機能は演算子オーバーロードのために追加されました。  
(this function added for operator overload)