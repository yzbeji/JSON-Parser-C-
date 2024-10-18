#include <tokenizer.h>

using namespace std;

void Tokenizer::PrintTokens()
{
	for (auto& token : tokens)
	{
		cout << token.value;
	}
}

void Tokenizer::SkipWhitespaces()
{
	while ((jsonString[this->index] == ' ' ||
		jsonString[this->index] == '\n' ||
		jsonString[this->index] == '\r' ||
		jsonString[this->index] == '\t') &&
		this->index < jsonString.size())
		this->index++;
}

void Tokenizer::Tokenize()
{
	SkipWhitespaces();
	while (this->index < jsonString.size())
	{
		Token token;
		switch (jsonString[this->index])
		{
		case '{':
		{
			token.type = Type::CURLY_BRACKET_OPEN;
			this->index += 1;
			token.value = '{';
			break;
		}
		case '}':
		{
			token.type = Type::CURLY_BRACKET_CLOSE;
			this->index += 1;
			token.value = '}';
			break;
		}
		case '[':
		{
			token.type = Type::SQUARE_BRACKET_OPEN;
			this->index += 1;
			token.value = '[';
			break;
		}
		case ']':
		{
			token.type = Type::SQUARE_BRACKET_CLOSE;
			this->index += 1;
			token.value = ']';
			break;
		}
		case 'n':
		{
			token.type = Type::NULL_VALUE;
			this->index += 4;
			token.value = "null";
			break;
		}
		case 't':
		{
			token.type = Type::BOOLEAN;
			token.value = "true";
			token.hasValue = true;
			this->index += 4;
			break;
		}
		case 'f':
		{
			token.type = Type::BOOLEAN;
			token.value = "false";
			token.hasValue = true;
			this->index += 5;
			break;
		}
		case ',':
		{
			token.type = Type::COMMA;
			token.value = ",";
			this->index += 1;
			break;
		}
		case ':':
		{
			token.type = Type::COLON;
			this->index += 1;
			token.value = ":";
			break;
		}
		case '"':
		{
			token.type = Type::STRING;
			token.hasValue = true;
			string value("");
			value += jsonString[this->index];
			this->index++;
			while (jsonString[this->index] != '"' && this->index < jsonString.size())
			{
				value += jsonString[this->index];
				this->index++;
			}
			try
			{
				if (this->index == jsonString.size())
				{
					throw runtime_error(R"(Strings should  start / end with '"'! \n)");
				}
				else
				{
					value += '"';
					this->index++;
				}
				token.value = value;
			}
			catch (exception& error)
			{
				printf("%s", error.what());
				exit(-1);
			}
			break;
		}
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			try
			{
				token.type = Type::NUMBER;
				token.value = jsonString[this->index];
				token.hasValue = true;
				this->index++;
				while ((jsonString[this->index] >= '0' && jsonString[this->index] <= '9') || jsonString[this->index] == '.')
				{
					token.value += jsonString[this->index];
					this->index++;
				}
				if (jsonString[this->index] == '+' ||
					jsonString[this->index] == '-' ||
					jsonString[this->index] == '.')
					throw runtime_error(R"(Numbers end with (0 -> 9)! \n)");
			}
			catch (exception& error)
			{
				printf("%s", error.what());
				exit(-1);
			}
			break;
		}
		}
		tokens.push_back(token);
		SkipWhitespaces();
	}
}
