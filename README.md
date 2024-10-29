Welcome to **yzbeji::json**, a lightweight and efficient JSON parsing library designed for C++ developers who need to easily integrate JSON handling into their applications. Whether you are developing desktop applications, embedded systems, or web services, yzbeji::json provides a simple and intuitive interface for parsing, manipulating, and serializing JSON data.


***How to use download and use it?*** \n

The library is now compatible with Windows only, and you need **vcpkg** and **git** to use it. \n
Assuming that you already have git installed, let me help you install vcpkg: \n

`cd your/directory` \n
`git clone https://github.com/microsoft/vcpkg.git` \n

Installed succesfully? Let's move on:  \n

`.\bootstrap-vcpkg.bat` \n
`./vcpkg install gtest` \n
`./vcpkg integrate install` \n

Tip: Just to be sure it was installed succesfully, run fast `./vcpkg list` and look for gtest. \n
Great, one more step to compile and combine all headers blah blah.. I used CMake, so, yeah, you'll need also CMake. (Promise it'll be the last one): \n
`./vcpkg install cmake` \n

Hope you're good for now. Now, let's clone this repository using `git clone -b main https://github.com/yzbeji/JSON-Parser-C-`! \n
Open Visual Studio (Code), "cmake" it and should compile smoothly. \n
You have an example in the **main.cpp* file what they do but I'll explain also what you can do for now. \n

So... \n

- Include the library with #include <json.h> and deserialize it using yzbeji::json class. \n 
- it has two constructors: one for a raw string parameter (try using raw string in C++ instead of normal string) and one with a file parameter (also recommend for this to open it in binary mode using std::ios::binary) \n
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
  \n
=> yzbeji::json j(rawString); (will detect if the json is incorrect and tell you what is wrong, but not very detailed yet) \n
=> j["a"]["b"]; (will detect also if the key or value you are trying to access is wrong) \n
=> will result an object named Wrapper that has a variant type variable. (variant = the variable can be more than only one type) \n
=> acces the string value (meaning the component) with casting. (Example: (std::string)j["a"]["b"] and it will result "[2,3,{c:test},[11,12]]") \n

- determine the size of the object / array using function GetSize() function; \n
- cast also possible integers and doubles (Example: (int)j["a"]["b"][0] and it will result 2) \n









