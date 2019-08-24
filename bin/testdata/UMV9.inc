//
//組み込みライブラリを隠蔽する(隠蔽側)
//

//_から始まる名前はloadされた時に公開されない
_load := load;

load := def(path)(...) {
    //api変数にラップして返すようにする
    api := object() <- _load(path);

    //パス文字列を変形して、
    //ライブラリの情報が定義されたファイルも読み込む
    info := object() <- _load(path + ".info");
};