# cmake-template
this project is template of programming by c In particular,  
specialized for cmake.

# make
````
cd src
cmake .
make
````

# config
must be replace a next words, in src/CMakeLists.txt
````
varAppName
varExecutable
varInstallDir
varIgnoreCompile
````


# how to use in vscode
first, install next plugin  
[C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

generate `c_cpp_properties.json` by next commad in command palette
(command palette show by Ctrl + Shift + P)
````
C/C++: Edit configurations (JSON)
````

open `c_cpp_properties.json`  
write include path, into include path section  
in generic, write a include path of was configured for cmake  
example:
````
"includePath": [
    "${workspaceFolder}/**",
    "/usr/local/Cellar/glib/2.60.0_1/lib/glib-2.0/include",
    "/usr/local/include/glib-2.0"
],
````