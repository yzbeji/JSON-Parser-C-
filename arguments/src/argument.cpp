#include <argument.h>

size_t argument::ExtractFunction()
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
	this->argumentType = functionName;
	if (offset == 0 && (argumentString[functionName.size()] != '(' || argumentString[argumentString.size() - 1] != ')'))
	{
		throw std::runtime_error("Use brackets for arguments");
	}
	if (offset == 0)
	{
		argumentString = argumentString.substr(functionName.size() + 1);	
		argumentString.pop_back();	
	}
	return offset;	
}	

void argument::ParseFunctionArguments(const yzbeji::json& j)	
{
	std::vector<double>numbers;
	unsigned int size = 0;
	unsigned int counterArguments = 0;

	if (ExtractFunction() == 0)
	{
		std::stringstream sstream(argumentString);
		std::string currentArgument;
		while (std::getline(sstream, currentArgument, ','))
		{
			Wrapper wrapper = j.ReturnInitialObject();	
			const Wrapper initialWrapper = wrapper;
			try
			{
				RemoveWhitespaces(currentArgument);
				double value = std::stod(currentArgument);
				if (counterArguments == 0 && argumentType == "size")
				{
					printf("%s", "Size works only for json");
					exit(-1);
				}
				else if (argumentType == "size" && counterArguments > 0)
				{
					printf("%s", "Only one argument allowed for size function \n");
					exit(-1);		
				}
				else if (argumentType != "size")
				{
					numbers.push_back(value);
					counterArguments++;
				}
			}
			catch (std::exception&)
			{
				if (argumentType == "size" && counterArguments > 0)
				{
					printf("%s", "Only one argument allowed for size function \n");
					exit(-1);
				}
				size_t position = 0;
				ParseObject(currentArgument, position, wrapper, initialWrapper);
				if (argumentType == "min" || argumentType == "max")
				{
					numbers.push_back((double)(wrapper));
				}
				else if (argumentType == "size" && counterArguments == 0)
				{
					size = wrapper.GetSize();
				}
			}
			counterArguments++;
		}

		if (numbers.empty() == false)
		{
			if (argumentType == "max")
			{
				std::cout << "Max value is: " << * std::max_element(numbers.begin(), numbers.end()) << std::endl;
			}
			else
			{
				std::cout << "Min value is: " << *std::min_element(numbers.begin(), numbers.end()) << std::endl;
			}
		}
		if (argumentType == "size")
		{
			std::cout << "Size is: " << size << std::endl;
		}
	}
	else
	{
		std::stringstream sstream(argumentString);
		std::string currentArgument;
		while (std::getline(sstream, currentArgument, ','))
		{
			Wrapper wrapper = j.ReturnInitialObject();	
			const Wrapper initialWrapper = wrapper;
			RemoveWhitespaces(currentArgument);
			size_t position = 0;
			ParseObject(currentArgument, position, wrapper, initialWrapper);
			std::cout << (std::string)wrapper << std::endl << std::endl;	
		}
	}
}

void argument::ParseObject(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper)
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

int argument::ParseArray(const std::string& argument, size_t& position, Wrapper& wrapper, const Wrapper& initialWrapper, bool& isHelperModified)
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