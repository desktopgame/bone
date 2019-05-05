# boneのビルド(build of bone)


## MacOSの場合(when on MacOS)
* まず、cmake, glibをインストールします。(in first, install the cmake and glib)
````
brew install glib
brew install cmake
````
* 次にCUnitをインストールします。(in next, install cunit)  
  * [CUnitについての備忘録](https://qiita.com/from_chc/items/db771bef1e83fc00783a)

次のコマンドを打ち込みます。(type next command in terminal)
````
cd bone
cmake .
make
````
## WindowsOSの場合(when on WindowsOS)
* まず、cmakeをインストールします。(in first, install the cmake)
  * [CMake](https://cmake.org/download/)
* 次に、visualstudioをインストールします。(in ndex, install the visualstudio)
  * [VisualStudio](https://docs.microsoft.com/ja-jp/visualstudio/install/install-visual-studio?view=vs-2019)
* CMakeを起動して、configure, generateの順に実行します。(launch cmake, execute in order a configure, generate)
  * visualstudioのプロジェクトが作成されます。(is created visual studio project)
  * glibのインクルードパス、ライブラリーパスを設定します。(setup a includePath and libraryPath for use to glib)
    * ナゲットを使うと簡単です。(use nuget to easy)
    * あとで詳細を記述...(will documented later...)
    * 実行時ディレクトリにglibと関連する .dll が必要です。