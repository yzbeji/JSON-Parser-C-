#pragma once
#include <tokenizer.h>
#include <variant>
#include <vector>
#include <type_traits>	
#include <stdio.h>
#include <unordered_map>
using namespace std;

// Promise to Object class that Array class exists
class Array; 
class Object
{
private:
	string rawObject;
	unordered_map<string, variant<string, int, bool, double, Array, Object>> objectValues;
public:
	string GetObject() const;
	variant<string, int, bool, double, Array, Object> operator[](const string value) const;
};

class Array
{
private:	
	string rawArray;
	vector<variant<string, int, bool, double, Array, Object>>arrayValues;
public:
	Array() : rawArray(R"([)") { }
	string GetArray() const;
	void AddToStringArray(const string& value) { rawArray += value; }
	// Funny 
	void AddToArrayNotAnArrayOrObject(const string& value, const Type& type);
	template<class T>
	void AddToArrayAnArrayOrObject(const T& ArrayOrObject) { arrayValues.push_back(ArrayOrObject); };
	variant<string, int, bool, double, Array, Object> operator[](int index) const;
};

// No implemented errors yet

class Parser
{
public:
	static Array ParseArray(vector<Token>::iterator& token)
	{
		Array array;
		while (token->type != Type::SQUARE_BRACKET_CLOSE)
		{
			switch (token->type)
			{
				case Type::COMMA:
				{
					array.AddToStringArray(token->value);
					token++;
					break;
				}
				case Type::SQUARE_BRACKET_OPEN:
				{
					const auto& arrayToAdd = ParseArray(++token);
					array.AddToArrayAnArrayOrObject<Array>(arrayToAdd);
					array.AddToStringArray(arrayToAdd.GetArray());
					break;
				}
				case Type::NUMBER:
				case Type::BOOLEAN:
				case Type::NULL_VALUE:
				case Type::STRING:
				{
					array.AddToStringArray(token->value);
					array.AddToArrayNotAnArrayOrObject(token->value, token->type);
					token++;
					break;
			}
			}
		}
		array.AddToStringArray(token->value);
		token++;
		return array;
	}
};

