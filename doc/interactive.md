# インタラクティブモード

インタラクティブモードは、他の言語で言う所の`REPL`,`irb` などに相当するものです。  
標準入力から 1 行読み取り、実行結果を即座に表示する...というのをループで実行します。  
bone ではコマンドライン引数を与えずに実行を開始するとこのモードで開始します。

## ショートカット

bone では利便性のために、このモードが開始するとまず`repl.bn`を読み込みます。  
ここによく使うショートカットなどを定義しておくと、簡単に関数や変数を参照できます。

## 複数行の入力

このモードでは 1 行づつ読み取るという性質上、プログラムが見づらくなることがあります。  
途中で改行してしまうと、正しく構文解析できないためです。

```
>>> i := 0;
>>> while(i < 10) { i += 1; }
```

この対策として、エスケープ(\\)で行が終了する場合、  
続けてもう 1 行入力を受け付けるようになっています。

```
>>> i := 0;
>>> while(i < 10) {\
>>>  i += 1\
>>> }
```
