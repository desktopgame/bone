# boneのビルド(build of bone)


## MacOSの場合(when on MacOS)
* まず、homebrewをインストールします。(in first, install homebrew)
    * [homebrew](https://brew.sh/index_ja.html)
* 次に、cmake, glibをインストールします。(in next, install the cmake and glib)
````
brew install glib
brew install cmake
````
* 次にCUnitをインストールします。(in next, install cunit)  
  * [CUnitについての備忘録](https://qiita.com/from_chc/items/db771bef1e83fc00783a)

次のコマンドを打ち込みます。(type next command in terminal)
````
cd your_clone_dir
cmake -D BONE_RUN=1 .
make
````

## WindowsOSの場合(when on WindowsOS)
* まず、cmakeをインストールします。(in first, install the cmake)
  * [CMake](https://cmake.org/download/)
* 次に、visualstudioをインストールします。(in ndex, install the visualstudio)
  * [VisualStudio](https://docs.microsoft.com/ja-jp/visualstudio/install/install-visual-studio?view=vs-2019)
* rubyをインストールします。
  * https://rubyinstaller.org/downloads/
* コマンドプロンプトから次を実行します。
````
cd your_clone_dir
ruby setup_sln.rb
````
* your_clone_dir\\..\\bone_slnが生成されます。
* その中のbone.slnを開く。
  * VisualStudioでは**DEBUG**ではなく **_DEBUG**としてマクロが定義されるので...
    * boneプロジェクトを右クリック -> プロパティ
    * 構成プロパティ -> C++ -> プリプロセッサ -> プリプロセッサの定義
    * **DEBUG**を追加