# プラグイン(plugin)

bone は実行時に外部のプラグインを読み込むことで挙動をカスタマイズできます。  
テスト時には testdata/plugins を参照し、  
それ以外の場合(リリースビルド)の場合には bone 本体と同じディレクトリの plugins ディレクトリを自動で参照します。

# インターフェイス

プラグインは bone 本体に対して決まった名前の関数をエクスポートする必要があります。  
例えば、libvarProjectName.dylib というプラグインが公開するべき関数は以下です。

```
void varProjectName_Init(bnInterpreter* bone);
void varProjectName_Destroy(bnInterpreter* bone);
const char* varProjectName_GetTargetVersion(bnInterpreter* bone);
```

## windows の場合

VisualStudio を使用して、
glib に加えて bone 自体のインクルードパス、ライブラリーパスを追加すれば可能ですが、  
手順はまだ自動化されていません。

## macOS の場合

- まず、homebrew をインストールします。
  - [homebrew](https://brew.sh/index_ja.html)
- 次に、cmake, glib をインストールします。

```
brew install glib
brew install cmake
```

次を実行して、bone のライブラリを生成します。

```
cd your_clone_dir/src
ruby buildall.rb Debug
もしくは
ruby buildall.rb Release
```

次を実行して、プラグインプロジェクトの雛形を生成します。

```
cd your_clone_dir
ruby plugin.rb myPlugin
```

your_clone_dir/../bone_plugin_myPlugin が生成されるので、  
これをベースとしてプログラムを書き換えていきます。

インクルードパスやライブラリパスは既に設定されており、  
ビルド可能な状態になっています。

```
cd your_clone_dir/../bone_plugin/src
cmake . -DCMAKE_BUILD_TYPE=Debug
もしくは
cmake . -DCMAKE_BUILD_TYPE=Release
make
```
