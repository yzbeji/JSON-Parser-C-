#pragma once
#include <iostream>
#include <string>
#include <vector>
#define UNDEFINED_VALUE "NO_VALUE"
using namespace std;

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
	string value;
	Type type;
	bool hasValue;
};

class Tokenizer
{
private:
	vector<Token> tokens;		
	string jsonString;
	size_t index;
	bool IsCharacterCorrect(const char& currentCharacter) const;
	void SkipWhitespaces();
public:
	vector<Token>::iterator DebuggerForJsonArray();
	vector<Token>::iterator DebuggerForJsonObject() { return tokens.begin(); }
	vector<Token>::const_iterator ReturnEnd() const { return tokens.end(); }
	Tokenizer(string jsonString) : jsonString(jsonString), index(0) { }
	void Tokenize();
	void PrintTokens() const;
};

