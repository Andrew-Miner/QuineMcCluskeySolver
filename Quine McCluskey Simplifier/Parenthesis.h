#pragma once
#include "Token.h"

class Parenthesis : public Token
{
public:
	static const int PARENTHESIS_TYPE = 6;

	Parenthesis(): right(true) {}
	Parenthesis(const std::string& face, bool rightParenthesis) : right(rightParenthesis), Token(face) {}
	virtual int getType() { return PARENTHESIS_TYPE; }

	bool isRight() { return right; }
	bool isLeft() { return !right; }

private:
	bool right;
};