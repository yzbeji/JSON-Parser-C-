#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tokenizer.h>
#include <parser.h>
// #include <json.h>
#include <argument.h>


int main(int argc, char* argv[])
{
	if(argc < 0)
	{
		printf("Missing input file");
		return -1;
	}
	else
	{
		std::cout << std::endl << std::endl;
		std::ifstream file(R"(C:\Users\Stefan\Desktop\jetbrains\jsonfiles\fourth-test.json)", std::ios::binary);
		yzbeji::json j(file);	
		Wrapper initialWrapper = j.ReturnInitialObject();
	    auto js = j.ReturnInitialObject();			
		//std::cout << (std::string)js << std::endl << std::endl;	
		//std::cout << (std::string)initialWrapper << std::endl << std::endl;	
		// if (argc == 2)
		// {
		//	 printf("%s", "What do you want to do?");
		//	 exit(-1);
		// }
		// std::cout << (std::string)j["web-app"]["servlet"][0] << std::endl;
		argument arg("size(web-app.servlet[0])");
		arg.ParseFunctionArguments(js, initialWrapper);
	}	
	return 0;
}

