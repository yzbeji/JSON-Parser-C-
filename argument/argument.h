#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>


class argument	
{
public:
	argument(const std::string& argument) : argumentString(argument) { }
private:
	std::vector<std::string> arguments;	
	std::string argumentString;	
	size_t ExtractFunction()		
	{
		size_t offset = std::string::npos;
		std::string functionName;
		functionName = ((offset = argumentString.find("min")) == 0) ? "min" : "UNDEFINED";
		if (functionName == "UNDEFINED") {
			functionName = ((offset = argumentString.find("max")) == 0) ? "max" : "UNDEFINED";
		}
		if (functionName == "UNDEFINED") {
			functionName = ((offset = argumentString.find("size")) == 0) ? "size" : "UNDEFINED";
		}
		this->arguments.push_back(functionName);
		if (offset == 0 && (argumentString[functionName.size()] != '(' || argumentString[argumentString.size() - 1] != ')'))
		{
			throw std::runtime_error("Use brackets for arguments");
		}
		if (offset == 0)
		{
			argumentString = argumentString.substr(functionName.size() + 1);
			argumentString[argumentString.size() - 1] = '\0';
		}
		return offset;
	}
	static std::string RemoveWhitespaces(const std::string& arg)
	{
		std::string result;
		for(auto& character: arg)
		{
			if (isalpha(character) || character == '.' || character == '[' || character == ']' || (character >= '0' && character <= '9'))
			{
				result += character;
			}
		}
		return result;
	}
public:
	void ParseFunctionArguments()
	{
		if (ExtractFunction() == 0)
		{
			std::stringstream sstream(argumentString);
			std::string currentArgument;
			while (std::getline(sstream, currentArgument, ','))
			{
				arguments.push_back(RemoveWhitespaces(currentArgument));
			}
		}
	}
};