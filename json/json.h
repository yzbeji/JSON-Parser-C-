#include <parser.h>
#include <memory>
#include <variant>
namespace yzbeji	
{
	typedef variant<string, int, bool, double, Array, Object> valueTypes;

	class json
	{
		private:
			Tokenizer tokenizer;
			unique_ptr<Object> object;
			vector<Token>::const_iterator token;
			vector<Token>::const_iterator begin;
			vector<Token>::const_iterator end;
		public:
			json(const string& jsonString);
			void operator [](const string& key)
			{	
				return;
			}
		private:
			void CheckForRemainingGarbageCharacters(const vector<Token>::const_iterator&, const vector<Token>::const_iterator&) const;
	};
}

