//
//ユーザが定義したライブラリを隠蔽する(ライブラリ側)
//

//_から始まる名前はloadされた時に公開されない
_file := object() <- load("file.bn");

print := def(str)() {
    _file.stdout.puts(str);
};