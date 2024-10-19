#include "parser.h"
#include <parser.h>


string Object::GetObject() const	
{
	return rawObject;
}

variant<string, int, bool, double, Array, Object> Object::operator[](const string value) const 
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

string Array::GetArray() const
{
	return rawArray;
}

void Array::AddToArrayNotAnArrayOrObject(const string& value, const Type& type) 
{
	switch (type)
	{
		case Type::NUMBER:
		{
			arrayValues.push_back(stod(value));
			break;
		}
		case Type::STRING:
		case Type::BOOLEAN:
		case Type::NULL_VALUE:
		{
			arrayValues.push_back(value);
			break;
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