#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tokenizer.h>
#include <parser.h>
#include <json.h>
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
		// std::ifstream file(argv[1], std::ios::binary);
		// yzbeji::json j(file);		
		// std::string js = (j["\"web-app\""]["\"servlet\""][0]["\"servlet-name\""][1]);			
		// std::cout << js << std::endl << std::endl;
		if (argc == 2)
		{
			printf("%s", "What do you want to do?");
			exit(-1);
		}
		argument arg("max(a.b[0], 10, a.b[1], 15)");
		arg.ParseFunctionArguments();
	}	
	return 0;
}

