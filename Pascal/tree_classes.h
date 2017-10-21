#pragma once
#include <string>
#include <vector>

class Node
{
public:
	Node()
	{
		parent = nullptr;
	}

	Node(Token got)
	{
		parent = nullptr;
		token = got;
	}
	~Node();

	Node *parent;
	std::vector<Node*> child;

	Token token;
};

class Statement : public Node
{
public:
	Statement() : Node()
	{

	}

	Statement(Token got) : Node(got)
	{

	}

	~Statement();
};

class ProgramHeading : public Statement
{
public:
	ProgramHeading() : Statement()
	{

	}

	ProgramHeading(Token got) : Statement(got)
	{

	}

	~ProgramHeading();
};

class Block : public Statement
{
public:
	Block() : Statement()
	{

	}

	Block(Token got) : Statement(got)
	{

	}

	~Block();
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

class IntConstant : public Constant
{
public:
	IntConstant(Token token) : Constant(token)
	{

	}
	~IntConstant();
};

class FloatConstant : public Constant
{
public:
	FloatConstant(Token token) : Constant(token)
	{

	}
	~FloatConstant();
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