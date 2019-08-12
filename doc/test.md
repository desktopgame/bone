# テスト

bone インタプリタそのもののテスト方法について。  
テストは現在以下の三種類があります。  
それぞれについて、成功を想定するテストコード、失敗を想定するテストコードが存在します。

- Parse
- VM
- Run

全てのテストコードは testdata 以下に配置されます。  
テストの種類をファイルから判別するため、ファイル名は以下の規則に従います。  
**Test_TypeExpectPanic?.in**

- Test
  - テストの名前を記述します。
- Type
  - Parse, VM, Run のどれかが入ります。
- Expect
  - Pass か Fail のどちらかが入ります。
  - 成功と失敗のどちらを期待するかを表します。
- Panic?
  - パニックによって終了することが期待される場合、ここに Panic と付け足します。

ファイル名の例

- Array_ParsePass.in
- Defer_RunPass.in
- HiddenVar_RunFail.in
- Panic_RunPassPanic.in

テストした結果得られた出力は testdata/out に出力されます。

# コマンドラインからテストを実行する

Debug ビルドでのみ、以下でテストを実行します。

```
bone
```
