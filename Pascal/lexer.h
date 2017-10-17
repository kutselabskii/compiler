#pragma once

//System includes
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <set>

enum States	//State codes
{
	IDLE,
	NAME,
	INT,
	FLOAT,
	STRING,
	OPERATOR,
	SEPARATOR,
	ERROR,
	COMMENT, 
	KEYWORD
};


enum Codes	//Symbol codes
{
	SPACE,
	LETTER,
	DIGIT,
	OP_SIGN,
	SEP_SIGN,
	QUOTE,
	UNKNOWN,
	COM_SIGN
};

const std::set <std::string> keywords = 
{	"and", "array", "asm", "begin", "break", "case", "const", "constructor", "continue", "destructor", 
	"div", "do", "downto", "else", "end", "false", "file", "for", "function", "goto", "if", "implementation", 
	"in", "inline", "interface", "label", "mod", "nil", "not", "object", "of", "on", "operator", "or", "packed",
	"procedure", "program", "record", "repeat", "set", "shl", "shr", "string", "then", "to", "true", "type",
	"unit", "until", "uses", "var", "while", "with", "xor"
};

//TODO make this table useful
const States fsm[100][100] = 
{
			       //Symbol
	//State          space + \n,   letter,   digit,    quote,      operator,    separator   unknown   comment
	/*IDLE*/         {IDLE,          NAME,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},
	/*NAME*/         {IDLE,          NAME,    NAME,     ERROR,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},
	/*INT*/          {IDLE,         ERROR,     INT,     ERROR,     OPERATOR,       ERROR,   ERROR,    COMMENT},
	/*FLOAT*/        {IDLE,         ERROR,   FLOAT,     ERROR,     OPERATOR,       ERROR,   ERROR,    COMMENT},
	/*STRING*/       {STRING,      STRING,  STRING,      IDLE,       STRING,      STRING,  STRING,     STRING},
	/*OPERATOR*/     {IDLE,          NAME,     INT,    STRING,        ERROR,   SEPARATOR,   ERROR,    COMMENT},
	/*SEPARATOR*/	 {IDLE,          NAME,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},

	/*ERROR*/        { }
};

typedef void (*FuncType)();

struct Token
{
	std::string token, type;
};

class Lexer
{
public:
	Lexer();
	~Lexer();

	bool fileAssign(std::string filename);	//Assign a file to lexer (logs)
	Token next();						//Parse assigned file (logs)

private:
	void (Lexer::*action_[100][100])();	//Array of function pointers (logs sometimes)

	std::ifstream file_;	//Assigned file to work with
	char file_buffer_[200];	//Current line in (^) file
	char *current_symbol_;	//Current symbol in (^) line

	bool eof_;

	int line_number_;
	int column_number_;

	States state_;				//Current state of a system
	std::string token_;		//Current token
	Token return_value;
	bool return_flag;

	Codes charIdentify_(char symbol); //Type of current_symbol_

	//Processing functions

	//Utility functions for the processing functions
	void pushAndStep_();
	void print_(std::string);
	void printTable_();

	//IDLE state
	void idleToIdle();
	void idleToName();
	void idleToInt();
	void idleToString();
	void idleToOperator();
	void idleToSeparator();
	void idleToError();
	void idleToComment();

	//NAME state
	void nameToIdle();
	void nameToName();
	/*void nameToInt(); No cases*/
	void nameToString();	//Error state
	void nameToOperator();
	void nameToSeparator();
	void nameToError();
	void nameToComment();

	//INT state
	void intToIdle();
	void intToName();	//Error state
	void intToInt();
	void intToString();	//Error state
	void intToOperator();
	void intToSeparator();
	void intToError();
	void intToComment();

	//FLOAT state
	void floatToIdle();
	void floatToName();
	void floatToFloat();
	void floatToString();
	void floatToOperator();
	void floatToSeparator();
	void floatToError();
	void floatToComment();

	//STRING state
	void stringToIdle();
	void stringToName();
	void stringToInt();
	void stringToString();
	void stringToOperator();
	void stringToSeparator();
	void stringToError();
	void stringToComment();

	//OPERATOR state
	void operatorToIdle();
	void operatorToName();
	void operatorToInt();
	void operatorToString();
	void operatorToOperator();
	void operatorToSeparator();
	void operatorToError();
	void operatorToComment();

	//SEPARATOR state
	void separatorToIdle();
	void separatorToName();
	void separatorToInt();
	void separatorToString();
	void separatorToOperator();
	void separatorToSeparator();
	void separatorToError();
	void separatorToComment();
};
