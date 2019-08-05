# コーディングスタイル

草案です

## 変数名

**bad style**

```
Window := CreateWindow();
Label := CreateLabel();
```

**good style**

```
window := create_window();
label := create_label();
```

## 戻り値の名前

**bad style**

```
plus := def(a, b)(num) {
    num := a + b;
};
```

**good style**

```
plus := def(a, b)(ret) {
    ret := a + b;
};
```

## ファイル名

**bad style**

```
MyFile.bn
```

**good style**

```
my_file.bn
```
