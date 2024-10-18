#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tokenizer.h>	

using namespace std;


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Missing input file! \n");
		return -1;
	}
	else
	{
		ifstream jsonFile(argv[1], ifstream::binary);
		if (jsonFile.fail()) 
		{
			printf("File was not found! \n");
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
		}
	}	
	return 0;
}

