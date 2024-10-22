#pragma once
#include <tokenizer.h>
#include <variant>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <unordered_map>


class Wrapper;
class Array;	

class ArrayOrObject
{
protected:
	std::string rawValue;
	ArrayOrObject(bool type) noexcept { rawValue = (type == 0) ? R"([)" : R"({)"; }
public:
	std::string GetRawValue() const noexcept { return rawValue; }
	void AddToRawValue(const std::string& value) noexcept { rawValue += value; }
	virtual void AddNotAnArrayOrObject(const std::vector<Token>::const_iterator&) = 0;
};



class Object : public ArrayOrObject
{
private:
	std::unordered_map<std::string, std::variant<std::string, int, bool, double, Array, Object>> objectValues;
public:
	Object() : ArrayOrObject(1) { }
	Wrapper operator[](const std::string& value) const;	
	void AddNotAnArrayOrObject(const std::vector<Token>::const_iterator& token) override;
	template<typename T>
	void AddAnArrayOrObject(const std::string& value, const T& ArrayOrObject) noexcept { objectValues.insert(make_pair(value, ArrayOrObject)); }
};

class Array : public ArrayOrObject
{
private:	
	std::vector<std::variant<std::string, int, bool, double, Array, Object>>arrayValues;
public:
	Array() : ArrayOrObject(0) { }
	void AddNotAnArrayOrObject(const std::vector<Token>::const_iterator& token) override;	
	Wrapper operator[](int index) const;	
	template<typename T>
	void AddAnArrayOrObject(const T& ArrayOrObject) noexcept { arrayValues.push_back(ArrayOrObject); }	
};

class Wrapper
{
public:
	std::variant<std::string, int, bool, double, Array, Object> value;
public:
	Wrapper& operator = (const Wrapper& wrapper) = delete;		
	Wrapper(const Wrapper& wrapper) = delete;			
	Wrapper(std::variant<std::string, int, bool, double, Array, Object> value) : value(value) { }
	Wrapper operator[](const char* key) const
	{
		return std::visit([&key](auto&& arg) -> Wrapper {
			if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Object>)
			{
				return Wrapper(static_cast<Object>(arg)[key]);		
			}
			else
			{
				throw std::bad_variant_access();		
			}
		}, value);
	}
	operator int()
	{
		return 1;
	}
	Wrapper operator[](const size_t& index) const
	{
		return std::visit([&index](auto&& arg) -> Wrapper  {
			if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Array>)
			{
				return Wrapper((static_cast<Array>(arg))[index]);	
			}
			else
			{
				throw std::bad_variant_access();	
			}
		}, value);
	}
	operator std::string() const
	{
		return std::visit([](auto&& arg) -> std::string {
			if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Object>)
			{
				return static_cast<Object>(arg).GetRawValue();
			}
			else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, Array>)
			{
				return static_cast<Array>(arg).GetRawValue();
			}
			else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>)
			{
				return arg;
			}
			else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, bool>)
			{
				return (arg == true) ? "true" : "false";
			}
			else if constexpr (std::is_same_v < std::decay_t<decltype(arg)>, int>)
			{
				return std::to_string(arg);	
			}
			else if constexpr (std::is_same_v < std::decay_t<decltype(arg)>, double> )
			{
				std::ostringstream sstream;
				sstream << arg;
				return sstream.str();	
			}
			throw std::bad_variant_access();	
		}, value);
	}
};

class Parser
{
private:
	static const std::vector<Token>::const_iterator ReturnPreviousToken(const std::vector<Token>::const_iterator& token)
	{
		return token - 1;
	}
	static const std::vector<Token>::const_iterator ReturnNextToken(const std::vector<Token>::const_iterator& token)
	{
		return token + 1;
	}
	static void IsCommaOrColonCorrect(const std::vector<Token>::const_iterator& token)
	{
		switch (ReturnPreviousToken(token)->type)	
		{
			case Type::CURLY_BRACKET_OPEN:
			case Type::SQUARE_BRACKET_OPEN:
			case Type::COMMA:
			case Type::COLON:
			{
				throw std::runtime_error("Before a comma there should be a value or an array or an object.");
			}
		}
		if (token->type == Type::COLON)
		{
			if (ReturnPreviousToken(token)->type != Type::STRING)
			{
				throw std::runtime_error("Before a colon only std::strings are allowed as keys");
			}
			switch (ReturnNextToken(token)->type)
			{
				case Type::CURLY_BRACKET_CLOSE:
				case Type::SQUARE_BRACKET_CLOSE:
				case Type::COMMA:
				{
					throw std::runtime_error("After colon it must be a value or object or array");
				}
			}
		}
	}
	static void IsOpenBracketCorrect(const std::vector<Token>::const_iterator& token)
	{
		switch (ReturnPreviousToken(token)->hasValue)
		{
			case true:
			{
				throw std::runtime_error("One of your open bracket is incorrect");
			}
		}
		if (token->type == Type::CURLY_BRACKET_OPEN && ReturnPreviousToken(token)->type == Type::CURLY_BRACKET_OPEN)
		{
			throw std::runtime_error("You can't open directly an object into an object without providing a key");
		}
		if (token->type == Type::CURLY_BRACKET_OPEN && (ReturnPreviousToken(token)->type != Type::COMMA && ReturnPreviousToken(token)->type != Type::COLON && ReturnPreviousToken(token)->type != Type::SQUARE_BRACKET_OPEN))
		{
			throw std::runtime_error("An object is followed by comma or colon only");	
		}
	}
	static void IsClosedBracketCorrect(const std::vector<Token>::const_iterator& token)
	{
		if (token->type == Type::SQUARE_BRACKET_CLOSE)
		{
			switch (ReturnPreviousToken(token)->type)
			{
				case Type::COMMA:
				case Type::COLON:
				case Type::CURLY_BRACKET_OPEN:	
				{
					throw std::runtime_error("One of your close square bracket is incorrect");
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
					throw std::runtime_error("One of your close curly bracket is incorrect");	
				}
			}
		}
		return;
	}
public:
	static Array ParseArray(std::vector<Token>::const_iterator& token, const std::vector<Token>::const_iterator& begin, const std::vector<Token>::const_iterator& end)
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
						if (ReturnPreviousToken(ReturnPreviousToken(token))->type == Type::COLON && token->type == Type::COMMA)
						{
							throw std::runtime_error(R"("Mot allowed to have "key":"value" inside in an array)");
						}
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
					case Type::CURLY_BRACKET_CLOSE:
					{
						throw std::runtime_error("Unexpected array closed incorrectly");
					}
				}
			}
			if (token != end)
			{
				IsClosedBracketCorrect(token);
				if (ReturnPreviousToken(ReturnPreviousToken(token))->type == Type::COLON)
				{
					throw std::runtime_error(R"(Not allowed to have "key":"value" inside in an array)");	
				}
				array.AddToRawValue(token->value);
				token++;
				return array;
			}
			else
			{
				throw std::runtime_error("One of the arrays is incorrect");
			}
		}
		catch (std::exception& error)	
		{
			printf("%s", error.what());
			exit(-1);
		}
	}
	static Object ParseObject(std::vector<Token>::const_iterator& token, const std::vector<Token>::const_iterator& begin,const std::vector<Token>::const_iterator& end)
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
								const std::string value = ReturnPreviousToken(token)->value;
								Array array = ParseArray(token += 2, begin, end);
								object.AddToRawValue(array.GetRawValue());
								object.AddAnArrayOrObject<Array>(value, array);
								break;
							}
							case Type::CURLY_BRACKET_OPEN:
							{
								const std::string value = ReturnPreviousToken(token)->value;
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
					case Type::SQUARE_BRACKET_OPEN:
					case Type::SQUARE_BRACKET_CLOSE:
					case Type::CURLY_BRACKET_OPEN:
					case Type::CURLY_BRACKET_CLOSE:
					{
						throw std::runtime_error("Unexpected bracket in wrong place");
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
				throw std::runtime_error("One of the objects is incorrect");
			}
		}
		catch (std::exception& error)	
		{
			printf("%s", error.what());
			exit(-1);
		}
	}
};
