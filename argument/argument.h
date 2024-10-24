#pragma once
#include <iostream>
#include <variant>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <json.h>
#include <parser.h>
#include <cctype>

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
	void ParseFunctionArguments(Wrapper& wrapper, const Wrapper& initialWrapper)	
	{	
		if (ExtractFunction() == 0)
		{
			std::stringstream sstream(argumentString);
			std::string currentArgument;
			while (std::getline(sstream, currentArgument, ','))
			{
				RemoveWhitespaces(currentArgument);
				size_t position = 0;
				ParseObject(currentArgument, position, wrapper, initialWrapper);
				std::cout << (std::string)wrapper << std::endl;		
				arguments.push_back("");
		
			}
		}
		if (arguments[0] == "size" && (arguments.size() > 2 || arguments.size() == 1))
		{
			throw std::runtime_error("Not enough / too many arguments");	
		}
	}
	
	void ParseObject(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper)
	{
		std::string component = ""; 
		Wrapper* helper = nullptr;
		while (position != argument.size())
		{
			switch (argument[position])
			{
				case '[':
				{
					if (component != "")
					{
						wrapper = wrapper[component.c_str()];
						component.clear();	
						component = R"()";	
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
		component;
		if (trim(component) != "")	
		{
			wrapper = wrapper[component.c_str()];	
		}
	}	
	static std::string trim(const std::string& str) {	
		size_t first = str.find_first_not_of(' ');	
		if (first == std::string::npos) 
			return "";	
		size_t last = str.find_last_not_of(' ');	
		return str.substr(first, (last - first + 1));	
	}	
	int ParseArray(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper, bool& isHelperModified)				
	{
		std::string component = "";
		Wrapper* helper = nullptr;
		while (position != argument.size() && argument[position] != ']')	
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
					component.clear();
					component = "";
					helper = new Wrapper(initialWrapper);
					bool secondIsHelperModified = false;
					int result = ParseArray(argument, ++position, (*helper), initialWrapper, secondIsHelperModified);	
					std::cout << (std::string)wrapper << std::endl;
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
		if (position != argument.size())	
		{
			position++;
		}
		try {
			return std::stoi(component);
		}
		catch(std:: exception&) {
			return -1;
		}
	}
	void GetSize(const std::string& argument)
	{

	}
};