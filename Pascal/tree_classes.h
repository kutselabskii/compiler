#pragma once
#include <string>


class Node
{
public:
	Node()
	{
		left = right = parent = nullptr;
	}

	Node(Token got)
	{
		left = right = nullptr;
		token = got;
	}
	~Node();

	Node *left, *right, *parent;
	Token token;
};

class Expression : public Node
{
public:
	Expression()
	{

	}

	Expression(Token token) : Node(token)
	{

	}
	~Expression();
};

class Variable : public Expression
{
public:
	Variable(Token token) : Expression(token)
	{

	}
	~Variable();
};

class Constant : public Expression
{
public:
	Constant(Token token) : Expression(token)
	{

	}
	~Constant();
};

class BinaryOperation : public Expression
{
public:
	BinaryOperation(Token token) : Expression(token)
	{

	}
	~BinaryOperation();
};

class UnaryOperation : public Expression
{
public:
	UnaryOperation(Token token) : Expression(token)
	{

	}
	~UnaryOperation();
};