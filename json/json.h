#include <parser.h>
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
		json(const std::string& jsonString);
		Wrapper operator [](const char* key) { return Wrapper((*object)[key]); }	
	private:
		void CheckForRemainingGarbageCharacters(const std::vector<Token>::const_iterator&, const std::vector<Token>::const_iterator&) const;
	};
}

