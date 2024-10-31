#include <tokenizer.h>	
#pragma once
#include <memory>
#include <variant>
#include <parser.h>
namespace yzbeji	
{
	class json
	{
	private:
		Tokenizer tokenizer;	
		std::unique_ptr<Object> object;
		std::vector<Token>::const_iterator token;
		std::vector<Token>::const_iterator begin;
		std::vector<Token>::const_iterator end;
	public:
		explicit json(const std::string& jsonString);
		explicit json(std::ifstream& jsonFile);
		Wrapper operator [](const char* key) { return Wrapper((*object)[key]); }
		Wrapper ReturnInitialObject() const { return Wrapper((*object)); }
	private:
		void CheckForRemainingGarbageCharacters(const std::vector<Token>::const_iterator&, const std::vector<Token>::const_iterator&) const;
	};
}

