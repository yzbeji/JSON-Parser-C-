#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tokenizer.h>
#include <parser.h>
#include <json.h>
using namespace std;
using json = yzbeji::json;
	
int main(int argc, char* argv[])
{
	if(argc < 0/*2*/)
	{
		printf("Missing input file");
		return -1;
	}
	else
	{
		ifstream jsonFile(R"(C:\Users\Stefan\Desktop\jetbrains\third-test.json)", ifstream::binary);
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
				
			string jsonString(buffer, length);	
			delete[] buffer;

			Tokenizer tokenizer(jsonString);
			tokenizer.Tokenize();
			tokenizer.PrintTokens();
			cout << endl << endl << endl;
		
			// Implement if iterator is not at the end there is garbage characters
			json j(jsonString);	
		}
	}	
	return 0;
}

