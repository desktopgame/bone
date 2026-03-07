# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## プロジェクト概要

bone はC言語で実装された動的型付けプロトタイピング言語のインタプリタ。独自機能として「オブジェクトインジェクション」「スコープインジェクション」「名前つき戻り値」を持つ。null やクラス、例外機構(try-catch)は意図的に排除されている。参考言語: crowbar, anko, Go, Ruby。

## ビルド

### macOS / Linux
```bash
cd src
cmake -D CMAKE_BUILD_TYPE=Debug -D BONE_RUN=1 .
make
```

### Windows
```cmd
ruby setup_sln.rb
# 親ディレクトリに bone_sln/bone.sln が生成される → Visual Studioで開く
# プリプロセッサに DEBUG を手動追加する必要あり
```

### CMake オプション
- `-DBONE_RUN=1`: 実行ファイルとしてビルド
- `-DBONE_SHARED=1`: 共有ライブラリとしてビルド
- `-DBONE_STATIC=1`: 静的ライブラリとしてビルド
- `-DDEF_NGLOBAL=ON`: プラグイン使用時のグローバル変数二重定義問題を回避

### 依存ライブラリ
- GLib 2.0 (`glib-2.0`)
- flex/bison (パーサ再生成時のみ: `src/bone/parse/gen.sh`)

## テスト

Debugビルドでは引数なしで `bone` を実行するとテストが走る:
```bash
cd bin
./bone
```

テストファイルは `bin/testdata/` に `.in` 拡張子で配置。ファイル命名規則: `TestName_TypeExpect[Panic].in`
- Type: `Parse`, `VM`, `Run`
- Expect: `Pass`, `Fail`
- 末尾に `Panic` を付けるとパニック終了を期待

一括テスト（CI相当）:
```bash
cd src
sh utest.sh
```

## アーキテクチャ

### ソースコード構成 (`src/bone/`)

- **parse/**: レキサ(`bone.l`) + パーサ(`bone.y`, bison) → AST(`ast.c`) → IL変換(`ast2il.c`)
- **il/**: 中間表現(IL)。式(`il_expr_*.c`)と文(`il_stmt_*.c`)のノード群。`il_toplevel` がトップレベル
- **runtime/**: VM実行エンジン
  - `interpreter.c`: インタプリタ本体。`bnInterpreter` が全状態のルート構造体
  - `vm.c`: 仮想マシン。`opcode.c` で定義されたオペコードを実行
  - `frame.c`: スタックフレーム。`enviroment.c` がコード環境を管理
  - `heap.c` / `storage.c`: メモリ管理・GC
  - `object.c`: オブジェクトシステム（プロトタイプベース）
  - 型: `integer.c`, `double.c`, `string.c`, `char.c`, `bool.c`, `array.c`, `lambda.c`, `any.c`
  - `extern/`: C バインディング（file, dir, regex, system, string_buffer, reflection）
- **util/**: ユーティリティ（string_pool, stack, memory, io, fmt, args, jump_stack）
- **main.c**: エントリポイント。Debug時はテスト実行、Release時はインタラクティブモードまたはファイル実行
- **test.c**: テストランナー

### 処理パイプライン

ソースコード → lexer/parser (flex/bison) → AST → IL (ast2il) → opcode生成 (compile) → VM実行

### bone スクリプト

- ファイル拡張子: `.bn`
- 標準ライブラリ相当は `bin/*.bn`（file.bn, array.bn, string.bn 等）
- `_` 始まりの変数はエクスポートされない（load/include時に非公開）

## コーディング規約

- C: clang-format 使用（Google スタイルベース、インデント幅 8）
- bone スクリプト: snake_case（変数名、ファイル名とも）、名前つき戻り値は `ret` を推奨
- C関数名: `bn` プレフィックス + PascalCase（例: `bnNewInterpreter`, `bnDeleteInterpreter`）
- GLib のデータ構造（GList, GHashTable, GString, GPtrArray）を全面的に使用
