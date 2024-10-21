#include <json.h>

using namespace std;
using namespace yzbeji;

json::json(const string& jsonString) : tokenizer(jsonString)
{
	try
	{
		tokenizer.Tokenize();
		begin = tokenizer.ReturnBegin();
		end = tokenizer.ReturnEnd();
		token = begin;
		object = make_unique<Object>(Parser::ParseObject(++token, begin, end));
		CheckForRemainingGarbageCharacters(token, end);
	}
	catch (exception& error)
	{
		printf("%s", error.what());
		exit(-1);
	}
}

void json::CheckForRemainingGarbageCharacters(const vector<Token>::const_iterator& token, const vector<Token>::const_iterator& end) const
{
	if (token != end)
	{
		throw runtime_error("Not useful characters found at the end");
	}
	return;
}
