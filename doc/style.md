# コーディングスタイル(coding style)
草案です(this is draft)  
将来変更されるかもしれません。(have possibility a update in future)

## 変数名(variable name)
**bad style**
````
Window := CreateWindow();
Label := CreateLabel();
````

**good style**
````
window := create_window();
label := create_label();
````

## 戻り値の名前(name of return value)
**bad style**
````
plus := def(a, b)(num) {
    num := a + b;
};
````

**good style**
````
plus := def(a, b)(ret) {
    ret := a + b;
};
````

## ファイル名(filename)
**bad style**
````
MyFile.bn
````

**good style**
````
my_file.bn
````