#pragma once

// BINARY OPERATION FUNCTORS
template <typename T>
class OrOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return operand1 | operand2; }
};

template <typename T>
class NorOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return ~(operand1 | operand2); }
};

template <typename T>
class XorOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return operand1 ^ operand2; }
};

template <typename T>
class AndOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return operand1 & operand2; }
};

template <typename T>
class NandOperation {
public:
	T operator()(const T& operand1, const T& operand2) const { return ~(operand1 & operand2); }
};

// UNARY OPERATION FUNCTORS
template <typename T>
class NotOperation {
public:
	T operator()(const T& operand) const { return ~operand; }
};