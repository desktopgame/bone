# C から bone を呼び出す

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
ruby buildall.rb
```

次を実行して、埋め込み済みプロジェクトの雛形を生成します。

```
cd your_clone_dir
ruby embed.rb
```

your_clone_dir/../bone_embed が生成されるので、  
これをベースとしてプログラムを書き換えていきます。

インクルードパスやライブラリパスは既に設定されており、  
ビルド可能な状態になっています。

```
cd your_clone_dir/../bone_embed/src
make
cd ../bin
./varProjectName
```
