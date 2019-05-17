# 組み込み変数(built-in variable)
## true
if, whileにおいて真であると判定される値です。

## false
if, whileにおいて偽であると判定される値です。

## argc
以下のように起動すると、 2 が渡ります。
````
$ bone hoge.bn arg arg2
````

## argv
以下のように起動すると、 ["arg", "arg2"] が渡ります。
````
$ bone hoge.bn arg arg2
````

## include
指定のパスをダイナミックスコープで読み込みます。  
つまり、includeの呼び出し側で定義された変数は読み込まれるファイルからも見えます。
````
def include(path)(...)
````

## load
指定のパスをローカルスコープで読み込みます。  
つまり、loadの呼び出し側で定義された変数は読み込まれるファイルから見えません。
````
def load(path)(...)
````

## object
空のオブジェクトを作成して返します。
==, != のみが定義されています。
````
def object()(ret)
````

## array
サイズを指定して配列を作成します。
配列リテラルは内部的にこれを呼び出します。
````
def array(length)(ret)
````

## string
charの配列から文字列を作成します。
文字列リテラルは内部的にこれを呼び出します。
````
def string(array)(ret)
````

## panic
実行中の関数を強制終了します。  
deferは実行されます。
````
def panic(message)()
````

## recover
もしパニックが起きていたら、メッセージを返します。
````
def recover()(ret)
````

## extern_var
ネイティブに定義された変数を返します。
````
def extern_var(name)(ret)
````

## extern_def
ネイティブに定義された関数を返します。  
指定された引数/戻り値が間違っているとパニックします。
````
def extern_def(name, params, returns)(ret)
````