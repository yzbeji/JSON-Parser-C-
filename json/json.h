#include <parser.h>
#include <tokenizer.h>
#include <memory>
#include <variant>
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
		Wrapper ReturnInitialObject() { return Wrapper((*object)); }
	private:
		void CheckForRemainingGarbageCharacters(const std::vector<Token>::const_iterator&, const std::vector<Token>::const_iterator&) const;
	};
}

