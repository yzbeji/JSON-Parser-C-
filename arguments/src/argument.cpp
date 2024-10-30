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
	const Wrapper initialWrapper = j.ReturnInitialObject();	
	std::vector<double>numbers;		
	std::vector<std::string>objects;	
	if (ExtractFunction() == 0)
	{
		std::stringstream sstream(argumentString);		
		std::string currentArgument;	
		while (std::getline(sstream, currentArgument, ','))
		{
			RemoveWhitespaces(currentArgument);
			try 
			{
				numbers.push_back(std::stod(currentArgument));	
			}
			catch (std::invalid_argument&)
			{
				objects.push_back(currentArgument);
			}
		}
		if (argumentType == "size" && (objects.size() == 0 || objects.size() > 1))
		{
			throw std::runtime_error("Only one argument is allowed for size");
		}
		else if (argumentType == "size" && objects.size() == 1 && numbers.size() == 0)
		{
			Wrapper wrapper = j.ReturnInitialObject();	
			size_t position = 0;	
			ParseObject(objects[0], position, wrapper, initialWrapper);
			std::cout << wrapper.GetSize() << std::endl;
		}
		else if ((argumentType == "min" || argumentType == "max") && (objects.size() > 0 || numbers.size() > 0))
		{
			if (objects.size() > 0)
			{
				std::vector<std::pair<Wrapper, size_t>> wrappers;
				std::vector<std::future<void>> futures;
				for (size_t i = 0; i < objects.size(); i++)
				{
					wrappers.push_back(std::make_pair(j.ReturnInitialObject(), 0));
				}
				for (size_t i = 0; i < objects.size(); i++)	
				{
					futures.push_back(std::async(std::launch::async, ParseObject, std::cref(objects[i]), std::ref(wrappers[i].second), std::ref(wrappers[i].first), std::cref(initialWrapper)));		
				}
				for (size_t i = 0; i < futures.size(); i++)
				{
					futures[i].get();	
					numbers.push_back((double)(wrappers[i].first));	
				}
			}
			if (argumentType == "max")	
			{
				std::cout << *std::max_element(numbers.begin(), numbers.end()) << std::endl;	
			}
			else
			{
				std::cout << *std::min_element(numbers.begin(), numbers.end()) << std::endl;	
			}
		}
		else if ((argumentType == "min" || argumentType == "max") && objects.size() == 0)
		{
			throw std::runtime_error("No arguments provided to the min / max function");
		}
	}
	else
	{
		std::stringstream sstream(argumentString);	
		std::string currentArgument;	
		while (std::getline(sstream, currentArgument, ','))	
		{
			RemoveWhitespaces(currentArgument);	
			objects.push_back(currentArgument);	
		}	
		if (objects.size() > 0)	
		{
			std::vector<std::pair<Wrapper, size_t>> wrappers;	
			std::vector<std::future<void>> futures;	
			for (size_t i = 0; i < objects.size(); i++)	
			{
				wrappers.push_back(std::make_pair(j.ReturnInitialObject(), 0));	
			}
			for (size_t i = 0; i < objects.size(); i++)	
			{	
				futures.push_back(std::async(std::launch::async, ParseObject, std::cref(objects[i]), std::ref(wrappers[i].second), std::ref(wrappers[i].first), std::cref(initialWrapper)));	
			}
			for (size_t i = 0; i < futures.size(); i++)	
			{
				futures[i].get();	
				std::cout << (std::string)wrappers[i].first << std::endl << std::endl;			
			}
		}
		else
		{
			throw std::runtime_error("No arguments provided");
		}
	}
}
