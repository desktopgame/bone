# about bone language

bone is based on prototyping language.  
and, added some peculiar idea.  
also, this language is but experiment,
so, included in other languages popular function is not included in this language.  
example:
* class, interface, overload
* method(use instead by closure)
* exception, and try-catch(instead of exception introduced a `panic`, `recover` and `defer`)
* null

# affected a language
* [crowbar](http://kmaebashi.com/programmer/devlang/crowbar.html)
* [anko](https://github.com/mattn/anko)
* [go](https://github.com/golang/go)

# feature
this language peculiar idea is object injection, and scope injection.  
this idea is related with named return value.  
and, need always nnamed return value if return a value.
## concrete example
named return value
````
f := def()(value, error) {
    value := "value is string";
    error := false;
};

// object injection
v := object() <- f();
println(v.value);
println(v.error);

// scope injection
{} <- f();
println(value);
println(error);

// nested named return value
f2 := def()(value, error, value2) {
    {}  <- f();
    value2 := "value 2";
};
````
object injection
````
std := object() <- load("./std.bn");
std.printf("hello, world");
````
scope injection
````
{} <- load("./std.bn");
printf("hello, world");
````

# more information
visit document folder if read more information