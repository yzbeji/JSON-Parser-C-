#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <json.h>
#include <parser.h>
#include <algorithm>	
#include <thread>
#include <future>


class argument	
{
public:
	argument(const std::string& argument) : argumentString(argument) { }
private:
	std::string argumentType;	
	std::string argumentString;	
	size_t ExtractFunction();
	static std::string RemoveWhitespaces(std::string& arg)
	{
		std::string result;
		for(auto& character: arg)
		{
			if (character != ' ' &&	
				character != '\n' &&	
				character != '\r' &&		
				character != '\t')	
			{
				result += character;
			}
		}
		return (arg = result);
	}
public:
	void ParseFunctionArguments(const yzbeji::json& j);
private:
	static void ParseObject(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper)	
	{
		std::string component = "";
		Wrapper* helper = nullptr;
		while (position < argument.size())
		{
			switch (argument[position])
			{
			case '[':
			{
				if (component != "")
				{
					wrapper = wrapper[component.c_str()];
					component.clear();
					component = "";
				}
				helper = new Wrapper(initialWrapper);
				bool isHelperModified = false;
				int result = ParseArray(argument, ++position, (*helper), initialWrapper, isHelperModified);
				wrapper = isHelperModified == true ? wrapper[static_cast<int>(*helper)] : wrapper[result];
				delete helper;
				break;
			}
			case '.':
			{
				if (component != "")
				{
					wrapper = wrapper[component.c_str()];
					component.clear();
					component = R"()";
				}
				position += 1;
				break;
			}
			case ']':
			{
				position += 1;
				break;
			}
			default:
			{
				component += argument[position];
				position += 1;
				break;
			}
			}
		}
		if (component != "")
		{
			wrapper = wrapper[component.c_str()];
		}
	}	
	static int ParseArray(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper, bool& isHelperModified)	
	{
		std::string component = "";
		Wrapper* helper = nullptr;
		while (position < argument.size() && argument[position] != ']')
		{
			switch (argument[position])
			{
			case '[':
			{
				if (component != "")
				{
					wrapper = wrapper[component.c_str()];
					component.clear();
					component = "";
				}
				helper = new Wrapper(initialWrapper);
				bool secondIsHelperModified = false;
				int result = ParseArray(argument, ++position, (*helper), initialWrapper, secondIsHelperModified);
				wrapper = (secondIsHelperModified == true) ? wrapper[static_cast<int>(*helper)] : wrapper[result];
				delete helper;
				break;
			}
			case '.':
			{
				if (component != "")
				{
					wrapper = wrapper[component.c_str()];
					component.clear();
					component = "";
					isHelperModified = true;
				}
				position += 1;
				break;
			}
			case ']':
			{
				position += 1;
				break;
			}
			default:
			{
				component += argument[position];
				position++;
				break;
			}
			}
		}
		try
		{
			if (position < argument.size() - 1)
			{
				position++;
			}
			return std::stoi(component);
		}
		catch (std::exception&)
		{
			return -1;
		}
	}
};