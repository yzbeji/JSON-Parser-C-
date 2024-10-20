#include <parser.h>

variant<string, int, bool, double, Array, Object> Object::operator[](const string& value) const	
{
	try
	{
		auto key = objectValues.find(value);
		if (key == objectValues.end())
			throw invalid_argument("Value does not exist");
		else
			return key->second;
	}
	catch (exception& error)
	{
		printf("%s", error.what());
		return -1;
	}
}

void Object::AddNotAnArrayOrObject(const vector<Token>::const_iterator& token)
{
	const vector<Token>::const_iterator& nextToken = token + 1;
	const vector<Token>::const_iterator& previousToken = token - 1;
	switch (nextToken->type)	
	{
		case Type::NUMBER:
		{
			objectValues.insert(make_pair(previousToken->value, stod(nextToken->value)));
			return;
		}
		case Type::STRING:
		case Type::BOOLEAN:
		case Type::NULL_VALUE:
		{
			objectValues.insert(make_pair(previousToken->value, nextToken->value));
			return;
		}
	}
}

void Array::AddNotAnArrayOrObject(const vector<Token>::const_iterator& token)
{
	switch (token->type)
	{
		case Type::NUMBER:
		{
			arrayValues.push_back(stod(token->value));
			return;
		}
		case Type::STRING:
		case Type::BOOLEAN:
		case Type::NULL_VALUE:
		{
			arrayValues.push_back(token->value);
			return;
		}
	}
}

variant<string, int, bool, double, Array, Object> Array::operator[](int index) const
{
	try
	{
		if (index < 0 || index > arrayValues.size())
			throw out_of_range("Invalid index");
		else
			return arrayValues[index];
	}
	catch (exception& error)
	{
		printf("%s", error.what());
	}
}