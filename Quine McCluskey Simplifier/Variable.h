#pragma once
#include "Token.h"

class Variable: public Token
{
public:
	static const int VARIABLE_TYPE = 5;

	Variable() {}
	Variable(const std::string& face) : Token(face) {}
	virtual int getType() { return VARIABLE_TYPE; }
	virtual char getVar() { return getToken()[0]; }
};