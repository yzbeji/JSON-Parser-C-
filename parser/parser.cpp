#include <parser.h>

Wrapper Object::operator[](const std::string& value) const	
{
	try
	{
		auto key = objectValues.find(value);
		if (key == objectValues.end())
			throw std::invalid_argument("Key or value that you are trying to access does not exist");
		else
			return Wrapper(key->second);
	}
	catch (std::exception& error)	
	{
		printf("%s", error.what());
		exit(-1);
	}
}

void Object::AddNotAnArrayOrObject(const std::vector<Token>::const_iterator& token)
{
	const std::vector<Token>::const_iterator& nextToken = token + 1;
	const std::vector<Token>::const_iterator& previousToken = token - 1;
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

void Array::AddNotAnArrayOrObject(const std::vector<Token>::const_iterator& token)
{
	switch (token->type)
	{
		case Type::NUMBER:
		{
			if ((token->value).find('.', 0) != std::string::npos)
			{
				arrayValues.push_back(stod(token->value));
			}
			else
			{
				std::istringstream sstream(token->value);
				int integer;
				sstream >> integer;
				arrayValues.push_back(integer);
			}
			return;
		}
		case Type::BOOLEAN:
		{
			arrayValues.push_back((token->value == "true") ? true : false);
			return;
		}
		case Type::STRING:
		case Type::NULL_VALUE:
		{
			arrayValues.push_back(token->value);
			return;
		}
	}
}

Wrapper Array::operator[](int index) const	
{
	try
	{
		if (index < 0 || index > arrayValues.size())
			throw std::out_of_range("Value in the array you are trying to acces does not exist");
		else
			return Wrapper(arrayValues[index]);	
	}
	catch (std::exception& error)
	{
		printf("%s", error.what());
	}
}