# プラグイン(plugin)
boneは起動時に同じディレクトリの .dylib を読み込み、  
ファイル名_Init, ファイル名_Destroyを適切なタイミングで実行する様になっています。  
これは動的リンクによって実装されています。  
  
example:
[bone-plugin](https://github.com/desktopgame/bone-plugin)