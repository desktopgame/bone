# プラグイン(plugin)
boneは実行時に外部のプラグインを読み込むことで挙動をカスタマイズできます。  
テスト時には testdata/plugins を参照し、  
それ以外の場合(リリースビルド)の場合にはbone本体と同じディレクトリのプラグインを自動で参照します。

# インターフェイス
プラグインはbone本体に対して決まった名前の関数をエクスポートする必要があります。  
例えば、libvarProjectName.dylibというプラグインが公開するべき関数は以下です。
````
void varProjectName_Init(bnInterpreter* bone);
void varProjectName_Destroy(bnInterpreter* bone);
````

## windowsの場合
VisualStudioを使用して、
glibに加えてbone自体のインクルードパス、ライブラリーパスを追加すれば可能ですが、  
手順はまだ自動化されていません。

## macOSの場合
* まず、homebrewをインストールします。
    * [homebrew](https://brew.sh/index_ja.html)
* 次に、cmake, glibをインストールします。
````
brew install glib
brew install cmake
````
次を実行して、boneのライブラリを生成します。
````
cd your_clone_dir/src
ruby buildall.rb
````
次を実行して、プラグインプロジェクトの雛形を生成します。
````
cd your_clone_dir
ruby plugin.rb
````
your_clone_dir/../bone_pluginが生成されるので、  
これをベースとしてプログラムを書き換えていきます。  

インクルードパスやライブラリパスは既に設定されており、  
ビルド可能な状態になっています。
````
cd your_clone_dir/../bone_plugin/src
make
````