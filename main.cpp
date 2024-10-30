#include <iostream>
#include <json.h>
#include <argument.h>

int main()
{
    std::string example = R"({
                                "glossary": {
                                    "title": "example glossary",
		                            "GlossDiv": {
                                        "title": "S",
			                            "GlossList": {
                                            "GlossEntry": {
                                                "ID": "SGML",
					                            "SortAs": "SGML",
					                            "GlossTerm": "Standard Generalized Markup Language",
					                            "Acronym": "SGML",
					                            "Abbrev": "ISO 8879:1986",
					                            "GlossDef": {
                                                    "para": "A meta-markup language, used to create markup languages such as DocBook.",
						                            "GlossSeeAlso": [0, 1, 2, 3, 4, 5, 6, "github.com", "yzbeji", "json", "parser"]
                                                },
					                            "GlossSee": "markup"
                                            }
                                        }
                                    }
                                }
                            })";
    // Example to use yzbeji::json library class
    yzbeji::json j(example);        
    // Output: [0, 1, 2, 3, 4, 5, 6, "github.com", "yzbeji", "json", "parser"] for json nested class
    std::cout << (std::string)(j.ReturnInitialObject())["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"] /* add [0] or [1] or ... to acces the array */ << std::endl << std::endl;
    // Example to use arguments class functions

    // Shows the maximum (also minimum works) between the input between brackets - Only integers allowed - Auto-conversion to integer for nested values in the json
    argument arg0(R"(max(glossary.GlossDiv.GlossList.GlossEntry.GlossDef.GlossSeeAlso[6], 3, glossary.GlossDiv.GlossList.GlossEntry.GlossDef.GlossSeeAlso[1]))");   

    // The size of the object / array - it also works for strings and integers etc. but it will be 1 by default
    argument arg1(R"(size(glossary.GlossDiv.GlossList.GlossEntry.GlossDef.GlossSeeAlso))");

    // This just outputs the raw values of it
    argument arg2(R"(glossary.GlossDiv.GlossList.GlossEntry.GlossDef.GlossSeeAlso[7], glossary, glossary.GlossDiv.GlossList)");  

    // Parsing the arguments and cout it
    arg0.ParseFunctionArguments(j); 
    std::cout << std::endl;
    arg1.ParseFunctionArguments(j);     
    std::cout << std::endl;
    arg2.ParseFunctionArguments(j); 
    std::cout << std::endl; 
    return 0;
}