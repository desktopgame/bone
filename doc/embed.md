# Cからboneを呼び出す(embed a bone to C)
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
次を実行して、埋め込み済みプロジェクトの雛形を生成します。
````
cd your_clone_dir
ruby embed.rb
````
your_clone_dir/../bone_embedが生成されるので、  
これをベースとしてプログラムを書き換えていきます。  

インクルードパスやライブラリパスは既に設定されており、  
ビルド可能な状態になっています。
````
cd your_clone_dir/../bone_embed/src
make
cd ../bin
./varProjectName
````