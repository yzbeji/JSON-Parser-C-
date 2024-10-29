#include <json.h>

using namespace yzbeji;	

yzbeji::json::json(const std::string& jsonString) : tokenizer(jsonString)		
{
	try
	{
		tokenizer.Tokenize();
		begin = tokenizer.ReturnBegin();
		end = tokenizer.ReturnEnd();
		token = begin;
		object = std::make_unique<Object>(Parser::ParseObject(++token, begin, end));
		CheckForRemainingGarbageCharacters(token, end);
	}
	catch (std::exception& error)
	{
		printf("%s", error.what());
		exit(-1);
	}
}

yzbeji::json::json(std::ifstream& jsonFile) : tokenizer(jsonFile)	
{
	try
	{
		tokenizer.Tokenize();
		begin = tokenizer.ReturnBegin();
		end = tokenizer.ReturnEnd();
		token = begin;
		object = std::make_unique<Object>(Parser::ParseObject(++token, begin, end));
		CheckForRemainingGarbageCharacters(token, end);
	}
	catch (std::exception& error)
	{
		printf("%s", error.what());
		exit(-1);
	}
}


void yzbeji::json::CheckForRemainingGarbageCharacters(const std::vector<Token>::const_iterator& token, const std::vector<Token>::const_iterator& end) const 
{
	if (token != end)
	{
		throw std::runtime_error("Not useful characters found at the end");
	}
	return;
}
	