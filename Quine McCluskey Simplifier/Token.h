#pragma once
#include <string>

class Token
{
public:
	static const int TOKEN_TYPE = 0;

	Token() {}
	Token(const std::string& face) : f(face) {}
	std::string getToken() { return f; }
	virtual int getType() { return TOKEN_TYPE; }

private:
	std::string f;

};

class UnknownToken : public Token
{
public:
	static const int UNKNOWN_TYPE = -1;

	UnknownToken() {}
	UnknownToken(const std::string& face) : Token(face) {}
	virtual int getType() { return UNKNOWN_TYPE; }
};