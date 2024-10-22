#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tokenizer.h>
#include <parser.h>
#include <json.h>

using json = yzbeji::json;
	
int main(int argc, char* argv[])
{
	if(argc < 0)
	{
		printf("Missing input file");
		return -1;
	}
	else
	{
		std::ifstream jsonFile(argv[1], std::ifstream::binary);
		if (jsonFile.fail()) 
		{
			printf("File was not found");
			return -1;
		}
		else
		{
			jsonFile.seekg(0, jsonFile.end);
			size_t length = jsonFile.tellg();
			jsonFile.seekg(0, jsonFile.beg);


			char* buffer = new char[length + 1];
			buffer[length] = '\0';
			
			jsonFile.read(buffer, length);
			jsonFile.close();
				
			std::string jsonString(buffer, length);	
			delete[] buffer;

			Tokenizer tokenizer(jsonString);
			tokenizer.Tokenize();
			tokenizer.PrintTokens();
			std::cout << std::endl << std::endl;	
			json j(jsonString);
			std::string js = j["\"web-app\""]["\"servlet\""];
			std::cout << js;
		}
	}	
	return 0;
}

