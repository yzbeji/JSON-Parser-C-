#pragma once
#include <tokenizer.h>
#include <variant>
#include <vector>
#include <type_traits>	
#include <stdio.h>
#include <unordered_map>
using namespace std;


class ArrayOrObject
{
protected:
	string rawValue;
	ArrayOrObject(bool type) { rawValue = (type == 0) ? R"([)" : R"({)"; }
public:
	string GetRawValue() const { return rawValue; }
	void AddToRawValue(const string& value) { rawValue += value; }
	virtual void AddNotAnArrayOrObject(const vector<Token>::const_iterator&) = 0;
};

class Array; 

class Object : public ArrayOrObject
{
private:
	unordered_map<string, variant<string, int, bool, double, Array, Object>> objectValues;
public:
	Object() : ArrayOrObject(1) { }
	variant<string, int, bool, double, Array, Object> operator[](const string& value) const;
	void AddNotAnArrayOrObject(const vector<Token>::const_iterator& token) override;
	template<typename T>
	void AddAnArrayOrObject(const string& value, const T& ArrayOrObject)
	{
		objectValues.insert(make_pair(value, ArrayOrObject));
	}
};

class Array : public ArrayOrObject
{
private:	
	vector<variant<string, int, bool, double, Array, Object>>arrayValues;
public:
	Array() : ArrayOrObject(0) { }
	void AddNotAnArrayOrObject(const vector<Token>::const_iterator& token) override;	
	variant<string, int, bool, double, Array, Object> operator[](int index) const;
	template<typename T>
	void AddAnArrayOrObject(const T& ArrayOrObject) { arrayValues.push_back(ArrayOrObject); }	
};


class Parser
{
private:
	static const vector<Token>::const_iterator ReturnPreviousToken(const vector<Token>::const_iterator& token)
	{
		return token - 1;
	}
	static const vector<Token>::const_iterator ReturnNextToken(const vector<Token>::const_iterator& token)
	{
		return token + 1;
	}
	static void IsCommaOrColonCorrect(const vector<Token>::const_iterator& token)
	{
		switch (ReturnPreviousToken(token)->type)	
		{
			case Type::CURLY_BRACKET_OPEN:
			case Type::SQUARE_BRACKET_OPEN:
			case Type::COMMA:
			case Type::COLON:
			{
				throw runtime_error("Before a colon or comma there should be a value or an array or an object");
			}
		}
	}
	static void IsOpenBracketCorrect(const vector<Token>::const_iterator& token)
	{
		switch (ReturnPreviousToken(token)->hasValue)
		{
			case true:
			{
				throw runtime_error("One of your open bracket is incorrect");
				return;
			}
		}
	}
	static void IsClosedBracketCorrect(const vector<Token>::const_iterator& token)
	{
		switch (ReturnPreviousToken(token)->type)
		{
			case Type::COMMA:
			case Type::COLON:
			case Type::CURLY_BRACKET_OPEN:
			{
				throw runtime_error("One of your close bracket is incorrect");
				return;
			}
		}
	}
public:
	static Array ParseArray(vector<Token>::const_iterator& token, vector<Token>::const_iterator& end)	
	{
		try
		{	
			IsOpenBracketCorrect(ReturnPreviousToken(token));	
			Array array;	
			while (token != end && token->type != Type::SQUARE_BRACKET_CLOSE)	
			{	
				switch (token->type)	
				{
					// Object not implemented yet, so just skip
					case Type::CURLY_BRACKET_CLOSE:
					{
						token++;
						break;
					}
					case Type::CURLY_BRACKET_OPEN:
					{
						token++;
						break;
					}
					case Type::COLON:
					case Type::COMMA:
					{
						IsCommaOrColonCorrect(token);
						array.AddToRawValue(token->value);
						token++;
						break;
					}
					case Type::SQUARE_BRACKET_OPEN:
					{
						Array arrayToAdd = ParseArray(++token, end);		
						array.AddAnArrayOrObject<Array>(arrayToAdd);	
						array.AddToRawValue(arrayToAdd.GetRawValue());
						break;
					}
					case Type::NUMBER:
					case Type::BOOLEAN:
					case Type::NULL_VALUE:
					case Type::STRING:
					{
						array.AddToRawValue(token->value);
						array.AddNotAnArrayOrObject(token);
						token++;	
						break;	
					}	
				}
			}
			if (token != end)
			{
				IsClosedBracketCorrect(token);
				array.AddToRawValue(token->value);
				token++;
				return array;
			}
			else
			{
				throw runtime_error("One of the arrays is incorrect");
			}
		}
		catch (exception& error)
		{
			printf("%s", error.what());
			exit(-1);
		}
	}
	static Object ParseObject(vector<Token>::const_iterator& token, vector<Token>::const_iterator& end)
	{
		try
		{
			IsOpenBracketCorrect(ReturnPreviousToken(token));
			Object object;
		}
		catch (exception& error)
		{
			printf("%s", error.what());
			exit(-1);
		}
	}
};
