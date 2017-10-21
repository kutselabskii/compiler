#pragma once

#include <iostream>
#include <string>

class LexerError : std::exception
{
public:
	LexerError(int line, int column, char c)
	{
		s = "(Line: " + std::to_string(line) + "; Column: " + std::to_string(column) + "): Unexpected symbol: '" + c + "'";
	}

	~LexerError()
	{

	}

	virtual const char* what() const override
	{
		return s.c_str();
	}

private:
	std::string s;
};


enum ParserErrorType
{
	OPERATOREXPECTED,
	IDEXPECTED,
	PARENTHESISEXPECTED,
	EXPRESSIONEXPECTED,
	SYMBOLEXPECTED,
	KEYWORDEXPECTED,
	CONSTANTEXPECTED
};

class ParserError : std::exception
{
public:
	ParserError(int line, int column, std::string token, int type)
	{
		s = "(Line: " + std::to_string(line) + "; Column: " + std::to_string(column) + "): ";
		switch (type)
		{
		case OPERATOREXPECTED:
			s += "Operator";
			break;
		case IDEXPECTED:
			s += "Identificator";
			break;
		case PARENTHESISEXPECTED:
			s += "')'";
			break;
		case EXPRESSIONEXPECTED:
			s += "Expression";
			break;
		case KEYWORDEXPECTED:
			s += "Keyword";
		case CONSTANTEXPECTED:
			s += "Constant";
		}
		s += " expected but '" + token + "' found";
	}


	ParserError(int line, int column, std::string token, int type, std::string symbol)
	{
		s = "(Line: " + std::to_string(line) + "; Column: " + std::to_string(column) + "): '";
		switch (type)
		{
			
		case SYMBOLEXPECTED:
			s += symbol;
			break;
		}
		s += "' expected but '" + token + "' found";
	}

	virtual const char* what() const override
	{
		return s.c_str();
	}

private:
	std::string s;
};