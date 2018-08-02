#pragma once
#include "Token.h"

class Integer : public Token
{
public:
	static const int INTEGER_TYPE = 4;

	Integer() : num(0) {}
	Integer(int number) : num(number), Token(std::to_string(number)) {}
	virtual int getType() { return INTEGER_TYPE; }
	int getValue() { return num; }

private:
	int num;
};