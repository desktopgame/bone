# 組み込み変数

## true

if, while において真であると判定される値です。

## false

if, while において偽であると判定される値です。

## argc

以下のように起動すると、 2 が渡ります。

```
$ bone hoge.bn arg arg2
```

## argv

以下のように起動すると、 ["arg", "arg2"] が渡ります。

```
$ bone hoge.bn arg arg2
```

## include

指定のパスをダイナミックスコープで読み込みます。  
つまり、include の呼び出し側で定義された変数は読み込まれるファイルからも見えます。

```
def include(path)(...)
```

## load

指定のパスをローカルスコープで読み込みます。  
つまり、load の呼び出し側で定義された変数は読み込まれるファイルから見えません。

```
def load(path)(...)
```

## eval

指定の文字列をダイナミックスコープで読み込みます。  
つまり、eval の呼び出し側で定義された変数は読み込まれるファイルからも見えます。

```
def eval(source)(...)
```

## object

空のオブジェクトを作成して返します。
==, != のみが定義されています。

```
def object()(ret)
```

## array

サイズを指定して配列を作成します。
配列リテラルは内部的にこれを呼び出します。

```
def array(length)(ret)
```

## string

char の配列から文字列を作成します。
文字列リテラルは内部的にこれを呼び出します。

```
def string(array)(ret)
```

## panic

実行中の関数を強制終了します。  
defer は実行されます。

```
def panic(message)()
```

## recover

もしパニックが起きていたら、メッセージを返します。

```
def recover()(ret)
```

## extern_var

ネイティブに定義された変数を返します。

```
def extern_var(name)(ret)
```

## extern_def

ネイティブに定義された関数を返します。  
指定された引数/戻り値が間違っているとパニックします。

```
def extern_def(name, params, returns)(ret)
```

## CWD

カレントワーキングディレクトリを表す文字列

## DIR

bone 実行ファイルのディレクトリを表す文字列
