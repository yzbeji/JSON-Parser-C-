#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <json.h>
#include <parser.h>
#include <algorithm>	

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
	void ParseObject(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper);
	int ParseArray(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper, bool& isHelperModified);
};