#include "tokenizer.h"
#include <tokenizer.h>
#include <fstream>
void Tokenizer::PrintTokens() const
{
	for (auto& token : tokens)
	{
		std::cout << token.value << " ";
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


Tokenizer::Tokenizer(std::ifstream& jsonFile) : index(0)	
{
	if (jsonFile.fail())
	{
		printf("File was not found");
		exit(-1);
	}
	else
	{
		jsonFile.seekg(0, jsonFile.end);
		size_t length = jsonFile.tellg();
		jsonFile.seekg(0, jsonFile.beg);
		char* buffer = new char[length + 1];
		buffer[length] = '\0';
		jsonFile.read(buffer, length);
		jsonFile.close();
		this->jsonString = buffer;
		delete[] buffer;
	}
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
		/* Assumed that you've written true & false correct */
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
		/* ----------------------------------------------- */
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
			std::string value("");
			value += jsonString[this->index];
			this->index++;
			while (this->index < jsonString.size() && jsonString[this->index] != '"')
			{
				value += jsonString[this->index];
				this->index++;
			}
			try
			{

				if ((this->index == jsonString.size()) || (index + 1 < jsonString.size() && IsCharacterCorrect(jsonString[index + 1]) == false))
				{
					throw std::runtime_error(R"(Strings should  start / end with '"')");
				}
				else
				{
					value += '"';
					this->index++;
				}
				token.value = value;
			}
			catch (std::exception& error)
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
				while ((jsonString[this->index] >= '0' && 
					    jsonString[this->index] <= '9') || 
					   (jsonString[this->index] == '.' && 
					   (jsonString[this->index - 1] >= '0' && 
						jsonString[this->index - 1] <= '9')))
				{
					token.value += jsonString[this->index];
					this->index++;
				}
				if (jsonString[this->index] == '+' ||
					jsonString[this->index] == '-' ||
					jsonString[this->index] == '.' ||
					jsonString[this->index - 1] == '.')
					throw std::runtime_error(R"(Numbers end with (0 -> 9))");
			}
			catch (std::exception& error)
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

constexpr bool Tokenizer::IsCharacterCorrect(const char& currentCharacter) const noexcept
{
	if (std::find(std::begin(correctCharacters), std::end(correctCharacters), currentCharacter) == std::end(correctCharacters))
		return false;
	else 
		return true;
}