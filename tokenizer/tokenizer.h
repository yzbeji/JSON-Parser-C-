#pragma once
#include <iostream>
#include <string>
#include <vector>
#define UNDEFINED_VALUE "NO_VALUE"


constexpr char correctCharacters[] = { '\r', '\n', '\t', ' ', '[', ']', '{', '}', ':', ',' };
enum class Type
{
	UNDEFINED,
	NULL_VALUE,
	STRING,
	NUMBER,
	BOOLEAN,
	CURLY_BRACKET_OPEN,
	CURLY_BRACKET_CLOSE,
	SQUARE_BRACKET_OPEN,
	SQUARE_BRACKET_CLOSE,
	COMMA,
	COLON
};

class Token
{
public:
	Token() : hasValue(false), type(Type::UNDEFINED), value(UNDEFINED_VALUE) { }
	std::string value;
	Type type;
	bool hasValue;
};

class Tokenizer
{
private:
	std::vector<Token> tokens;		
	std::string jsonString;
	size_t index;
	constexpr bool IsCharacterCorrect(const char& currentCharacter) const noexcept;
	void SkipWhitespaces();
public:
	std::vector<Token>::const_iterator ReturnBegin() const noexcept { return tokens.begin(); }
	std::vector<Token>::const_iterator ReturnEnd() const noexcept{ return tokens.end(); }
	Tokenizer(std::string jsonString) : jsonString(jsonString), index(0) { }
	void Tokenize();
	void PrintTokens() const;
};

