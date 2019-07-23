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
cd bone
cmake .
make
````

## WindowsOSの場合(when on WindowsOS)
* まず、cmakeをインストールします。(in first, install the cmake)
  * [CMake](https://cmake.org/download/)
* 次に、visualstudioをインストールします。(in ndex, install the visualstudio)
  * [VisualStudio](https://docs.microsoft.com/ja-jp/visualstudio/install/install-visual-studio?view=vs-2019)
* コマンドプロンプトから次を実行します。
````
cmake -S your_clone_dir\src -B your_solution_dir -D BONE_RUN=1
````
または
````
cmake -D BONE_RUN=1 -S your_clone_dir\src -B your_solution_dir
````
* 作成されたプロジェクト(your_solution_dir\bone.sln)を開き  
  メニューより　ツール -> Nugetパッケージマネージャ -> パッケージマネージャコンソール　を開く
````
Install-Package glib -ProjectName bone
````
* PowerShellから次を実行します。
````
cd your_solution_dir
mkdir include
Invoke-WebRequest -Uri https://gist.githubusercontent.com/tversteeg/ba848fad54a96754b64a316ca91b4968/raw/5b4e66b7561e092ebcfe4f3fcef478550b5e4ecc/glibconfig.h -OutFile include/glibconfig.h
Invoke-WebRequest -Uri https://gist.githubusercontent.com/desktopgame/8083490657442cbde305d615399346a4/raw/b0b9c376f6eb1bc2491d70cefa8463be2792d849/vs_patch.rb -OutFile vs_patch.rb
````
* VisualStudioを閉じる
* rubyが実行できるなら、vs_patch.rbを実行して  
  bone.vcxprojの一部の設定を置換する
    ````
    cd your_solution_dir
    ruby vs_patch.rb
    ````
* ruby
  * https://rubyinstaller.org/downloads/
* VisualStudioを開き、ビルド