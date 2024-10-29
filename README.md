Welcome to **yzbeji::json**, a lightweight and efficient JSON parsing library designed for C++ developers who need to easily integrate JSON handling into their applications. Whether you are developing desktop applications, embedded systems, or web services, yzbeji::json provides a simple and intuitive interface for parsing, manipulating, and serializing JSON data.


***How to use download and use it?***

The library is now compatible with Windows only, and you need **vcpkg** and **git** to use it.
Assuming that you already have git installed, let me help you install vcpkg:

`cd your/directory`
`git clone https://github.com/microsoft/vcpkg.git`

Installed succesfully? Let's move on: 

`.\bootstrap-vcpkg.bat`
`./vcpkg install gtest`
`./vcpkg integrate install`

Tip: Just to be sure it was installed succesfully, run fast `./vcpkg list` and look for gtest. \n
Great, one more step to compile and combine all headers blah blah.. I used CMake, so, yeah, you'll need also CMake. (Promise it'll be the last one):
`./vcpkg install cmake`

Hope you're good for now. Now, let's clone this repository using `git clone -b main https://github.com/yzbeji/JSON-Parser-C-`!
Open Visual Studio (Code), "cmake" it and should compile smoothly.
You have an example in the **main.cpp* file what they do but I'll explain also what you can do for now.

So...

- Include the library with #include <json.h> and deserialize it using yzbeji::json class.
- it has two constructors: one for a raw string parameter (try using raw string in C++ instead of normal string) and one with a file parameter (also recommend for this to open it in binary mode using std::ios::binary)
- access nested classes using index operator []. Example:
rawString = {
              "a": {
                "b": [
                  2,
                  3,
                  { "c": "test" },
                  [ 11, 12 ]
                ]
              }
            } 
=> yzbeji::json j(rawString); (will detect if the json is incorrect and tell you what is wrong, but not very detailed yet)
=> j["a"]["b"]; (will detect also if the key or value you are trying to access is wrong)
=> will result an object named Wrapper that has a variant type variable. (variant = the variable can be more than only one type)
=> acces the string value (meaning the component) with casting. (Example: (std::string)j["a"]["b"] and it will result "[2,3,{c:test},[11,12]]")

- determine the size of the object / array using function GetSize() function;
- cast also possible integers and doubles (Example: (int)j["a"]["b"][0] and it will result 2)









