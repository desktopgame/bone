# bone のビルド

## MacOS の場合

- まず、homebrew をインストールします。
  - [homebrew](https://brew.sh/index_ja.html)
- 次に、cmake, glib をインストールします。

```
brew install glib
brew install cmake
```

次のコマンドを打ち込みます。

```
cd your_clone_dir/src
cmake -D BONE_RUN=1 .
make
```

インストールするなら、次も実行します。

```
cd your_clone_dir/src
brew install doxygen
sudo make install
sudo make _add_path
```

## WindowsOS の場合

- まず、cmake をインストールします。
  - [CMake](https://cmake.org/download/)
- 次に、visualstudio をインストールします。
  - [VisualStudio](https://docs.microsoft.com/ja-jp/visualstudio/install/install-visual-studio?view=vs-2019)
- ruby をインストールします。
  - https://rubyinstaller.org/downloads/
- コマンドプロンプトから次を実行します。

```
cd your_clone_dir
ruby setup_sln.rb
```

- your_clone_dir\\..\\bone_sln が生成されます。
- その中の bone.sln を開く。
  - VisualStudio では**DEBUG**ではなく **\_DEBUG**としてマクロが定義されるので...
    - bone プロジェクトを右クリック -> プロパティ
    - 構成プロパティ -> C++ -> プリプロセッサ -> プリプロセッサの定義
    - **DEBUG**を追加
- インストーラはまだ実装されていません。

# NGLOBAL マクロ

bone ではプラグインの仕組みを提供しています。  
その時最低でも以下のファイルが必要になります。

- bone 実行ファイル(BONE-APP)
- bone ライブラリファイル(BONE-DLL)
- プラグインライブラリファイル(PLUGIN-DLL)

この時、PLUGIN-DLL は BONE-DLL に対してリンクされます。  
つまり、PLUGIN-DLL から bone の関数を呼び出しても、実際に起動するのは BONE-APP ではなく BONE-DLL の方です。  
これが何を引き起こすかというと、同じグローバル変数が二つ存在することになります。  
この挙動が不都合になる場合があるので、NGLOBAL マクロで無効にできるようになっています。
