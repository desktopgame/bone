# トリック(trick)
* includeされるファイル = includeCallee
* includeするファイル = includeCaller
* loadされるファイル = loadCallee
* loadするファイル = loadCaller

## load関数をフックする(hook a load function)
**includeCallee**
````
orig_load := load;

load := def(path)(...) {
    dumpTable();
    {} <- orig_load(path);
    println(path);
};
````
**includeCaller**
````
{} <- include("includeCallee");
{} <- load("your file path");
````
**OUTPUT**
````
your file path
````