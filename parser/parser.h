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
	void AddAnArrayOrObject(const string& value, const T& ArrayOrObject) { objectValues.insert(make_pair(value, ArrayOrObject)); }
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
				throw runtime_error("Before a comma there should be a value or an array or an object.");
			}
		}
		if (token->type == Type::COLON)
		{
			if (ReturnPreviousToken(token)->type != Type::STRING)
			{
				throw runtime_error("Before a colon only strings are allowed as keys");
			}
			switch (ReturnNextToken(token)->type)
			{
				case Type::CURLY_BRACKET_CLOSE:
				case Type::SQUARE_BRACKET_CLOSE:
				case Type::COMMA:
				{
					throw runtime_error("After colon it must be a value or object or array");
				}
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
			}
		}
		if (token->type == Type::CURLY_BRACKET_OPEN && ReturnPreviousToken(token)->type == Type::CURLY_BRACKET_OPEN)
		{
			throw runtime_error("You can't open directly an object into an object without providing a key");
		}
	}
	static void IsClosedBracketCorrect(const vector<Token>::const_iterator& token)
	{
		if (token->type == Type::SQUARE_BRACKET_CLOSE)
		{
			switch (ReturnPreviousToken(token)->type)
			{
				case Type::COMMA:
				case Type::COLON:
				case Type::CURLY_BRACKET_OPEN:	
				{
					throw runtime_error("One of your close square bracket is incorrect");
				}
			}
		}
		else if(token->type == Type::CURLY_BRACKET_CLOSE)
		{
			switch (ReturnPreviousToken(token)->type)
			{
				case Type::COMMA:
				case Type::COLON:
				case Type::SQUARE_BRACKET_OPEN:	
				{
					throw runtime_error("One of your close curly bracket is incorrect");	
				}
			}
		}
		return;
	}
public:
	static Array ParseArray(vector<Token>::const_iterator& token, const vector<Token>::const_iterator& begin, const vector<Token>::const_iterator& end)
	{
		try
		{	
			IsOpenBracketCorrect(ReturnPreviousToken(token));	
			Array array;	
			while (token != end && token->type != Type::SQUARE_BRACKET_CLOSE)	
			{	
				switch (token->type)	
				{
					case Type::CURLY_BRACKET_OPEN:
					{
						Object objectToAdd = ParseObject(++token, begin, end);
						array.AddToRawValue(objectToAdd.GetRawValue());
						array.AddAnArrayOrObject<Object>(objectToAdd);
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
						Array arrayToAdd = ParseArray(++token, begin, end);		
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
	static Object ParseObject(vector<Token>::const_iterator& token, const vector<Token>::const_iterator& begin,const vector<Token>::const_iterator& end)
	{
		try
		{
			if(token != begin)
			{
				IsOpenBracketCorrect(token);
			}
			if (ReturnPreviousToken(token) != begin)
			{
				IsOpenBracketCorrect(ReturnPreviousToken(token));
			}
			Object object;
			while (token != end && token->type != Type::CURLY_BRACKET_CLOSE)
			{
				switch (token->type)
				{
					case Type::COLON:
					{
						IsCommaOrColonCorrect(token);
						object.AddToRawValue(token->value);
						switch(ReturnNextToken(token)->type)
						{
							case Type::SQUARE_BRACKET_OPEN:
							{
								const string value = ReturnPreviousToken(token)->value;
								Array array = ParseArray(token += 2, begin, end);
								object.AddToRawValue(array.GetRawValue());
								object.AddAnArrayOrObject<Array>(value, array);
								break;
							}
							case Type::CURLY_BRACKET_OPEN:
							{
								const string value = ReturnPreviousToken(token)->value;
								Object objectToAdd = ParseObject(token += 2, begin, end);
								object.AddToRawValue(objectToAdd.GetRawValue());
								object.AddAnArrayOrObject<Object>(value, objectToAdd);
								break;
							}
							case Type::BOOLEAN:
							case Type::NULL_VALUE:
							case Type::NUMBER:
							case Type::STRING:
							{
								object.AddToRawValue(ReturnNextToken(token)->value);
								object.AddNotAnArrayOrObject(token);
								token += 2;
								break;
							}
						}
						break;
					}
					case Type::COMMA:	
					{
						IsCommaOrColonCorrect(token);
						object.AddToRawValue(token->value);
						token++;
						break;
					}
					case Type::STRING:
					{
						object.AddToRawValue(token->value);
						token++;
						break;
					}
				}
			}
			if (token != end)
			{
				IsClosedBracketCorrect(token);
				object.AddToRawValue(token->value);
				token++;
				return object;
			}
			else
			{
				throw runtime_error("One of the objects is incorrect");
			}
		}
		catch (exception& error)
		{
			printf("%s", error.what());
			exit(-1);
		}
	}
};
