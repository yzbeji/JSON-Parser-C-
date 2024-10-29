Welcome to **yzbeji::json**, a lightweight and efficient JSON parsing library designed for C++ developers who need to easily integrate JSON handling into their applications. Whether you are developing desktop applications, embedded systems, or web services, yzbeji::json provides a simple and intuitive interface for parsing, manipulating, and serializing JSON data.


***How to use download and use it?*** <br>

The library is now compatible with Windows only, and you need **vcpkg** and **git** to use it. <br>
Assuming that you already have git installed, let me help you install vcpkg: \n

`cd your/directory` <br>
`git clone https://github.com/microsoft/vcpkg.git` <br>

Installed succesfully? Let's move on:  <br>

`.\bootstrap-vcpkg.bat` <br>
`./vcpkg install gtest` <br>
`./vcpkg integrate install` <br>

Tip: Just to be sure it was installed succesfully, run fast `./vcpkg list` and look for gtest. <br>
Great, one more step to compile and combine all headers blah blah.. I used CMake, so, yeah, you'll need also CMake. (Promise it'll be the last one): <br>
`./vcpkg install cmake` <br>

Hope you're good for now. Now, let's clone this repository using `git clone -b main https://github.com/yzbeji/JSON-Parser-C-`! <br>
Open Visual Studio (Code), "cmake" it and should compile smoothly. <br>
You have an example in the **main.cpp* file what they do but I'll explain also what you can do for now. <br>

So... <br>

- Include the library with #include <json.h> and deserialize it `using yzbeji::json class`. <br>
- it has two constructors: one for a raw string parameter (try using raw string in C++ instead of normal string) and one with a file parameter (also recommend for this to open it in binary mode using std::ios::binary) <br>
- access nested classes using index operator []. Example:
`rawString = {
              "a": {
                "b": [
                  2,
                  3,
                  { "c": "test" },
                  [ 11, 12 ]
                ]
              }
            }`
  <br>
=> ```yzbeji::json j(rawString)```; (will detect if the json is incorrect and tell you what is wrong, but not very detailed yet) <br>
=> ```j["a"]["b"]```; (will detect also if the key or value you are trying to access is wrong) <br>
=> will result an object named Wrapper that has a variant type variable. (variant = the variable can be more than only one type) <br>
=> acces the string value (meaning the component) with casting. (Example: `(std::string)j["a"]["b"]` and it will result `"[2,3,{c:test},[11,12]]"`) <br>

- determine the size of the object / array using function `GetSize()` function; <br>
- cast also possible integers and doubles (Example: `(int)j["a"]["b"][0]` and it will result 2) <br>

So... x2 <br>

I have also implemented some functions using a separated class named arguments. Here's what you can do: <br>

- size() => Implements GetSize for a json class but you need only json file / raw string and inside the brackets the argument. (Example: ```arg(size(a.b))``` outputs the number of the elements of the array)
- max/min() => Determines the maximum/minimum element from the parameters inside the brackets separated by comma (Example: ```arg(max(a.b[0], a.b[1], -15)``` outputs 3 and yes, if it is not a json it will cast it to an integer)
- just some nested jsons => show the values for arrays / objects or the value of a string / integer / double / bool value (Example: ```arg(a, a.b, a.b[3],..)``` outputs value of key ***a***, ***b*** and the array ***[11, 12]*** which is owned by the array value of ***b***  









