#pragma once
#include "token.h"
#include <vector>
#include <functional>

template <typename T>
class DefaultOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return operand1; }
};

template <typename T>
class DefaultUnaryOperation {
public:
	T operator()(const T& operand) const { return operand; }
};

template <typename T>
class Operator : public Token
{
public:
	static const int OPERATOR_TYPE = 1;
	static const bool LEFT_ASSOCIATIVE = true;
	static const bool RIGHT_ASSOCIATIVE = false;

	std::function<T(T, T)> op;

	Operator() : prec(0) {}
	Operator(const std::string& face, const std::function<T(T,T)> operation, const int& precedence, const bool& associativity) : op(operation), prec(precedence), assoc(associativity), Token(face) {}
	int getPrecedence() { return prec; }
	bool isLeftAssociative() { return assoc == LEFT_ASSOCIATIVE; }
	bool isRightAssociative() { return prec == RIGHT_ASSOCIATIVE; }
	virtual int operandCount() { return 2; }
	virtual int getType() { return OPERATOR_TYPE; }
	T operator()(const std::vector<T>& operands) { return op(operands[0], operands[1]); }
	T operator()(const T& operand1, const T& operand2) { return op(operand1, operand2); }

private:
	int prec;		// Precedence
	bool assoc;		// Associativity
};

template <typename T>
class UnaryOperator : public Operator<T>
{
public:
	static const int UNARY_TYPE = 2;
	std::function<T(T)> unaryOp;

	UnaryOperator() {}
	UnaryOperator(const std::string& face, const std::function<T(T)> operation, const int& precedence, const bool& associativity)
		: unaryOp(operation), Operator<T>(face, DefaultOperation<T>(), precedence, associativity) {}

	virtual int operandCount() { return 1; }
	virtual int getType() { return UNARY_TYPE; }

	T operator()(const T& operand) { return unaryOp(operand); }
	T operator()(const std::vector<T>& operands) { return unaryOp(operands[0]); }
};

template <typename T>
class BinaryOperator : public Operator<T>
{
public:
	static const int BINARY_TYPE = 3;
	BinaryOperator() {}
	BinaryOperator(std::string face, const std::function<T(T)> operation, const int& precedence, const bool& associativity) : Operator<T>(face, operation, precedence, associativity) {}
	T operator()(const T& operand1, const T& operand2) { return this->op(operand1, operand2); }
	virtual int getType() { return BINARY_TYPE; }
};
