#pragma once

//System includes
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>

#include "Exceptions.h"
#include "enums.h"

//TODO make this table useful
const States fsm[100][100] = 
{
			       //Symbol
	//State          space + \n,   letter,   digit,    quote,      operator,    separator   unknown   comment
	/*IDLE*/         {IDLE,          ID,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},
	/*ID*/			 {IDLE,          ID,    ID,     ERROR,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},
	/*INT*/          {IDLE,         ERROR,     INT,     ERROR,     OPERATOR,       ERROR,   ERROR,    COMMENT},
	/*FLOAT*/        {IDLE,         ERROR,   FLOAT,     ERROR,     OPERATOR,       ERROR,   ERROR,    COMMENT},
	/*STRING*/       {STRING,      STRING,  STRING,      IDLE,       STRING,      STRING,  STRING,     STRING},
	/*OPERATOR*/     {IDLE,          ID,     INT,    STRING,        ERROR,   SEPARATOR,   ERROR,    COMMENT},
	/*SEPARATOR*/	 {IDLE,          ID,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR,    COMMENT},

	/*ERROR*/        { }
};

typedef void (*FuncType)();

struct Token
{
	States type;
	std::string token;
};

class Lexer
{
public:
	Lexer(std::string);
	~Lexer();

	bool fileAssign(std::string fileID);	
	Token next();						
	Token current();
	const int getLine() const { return _line_number; }
	const int getColumn() const { return _column_number; }

private:
	void (Lexer::*_action[100][100])();	//Array of function pointers (logs sometimes)

	std::string _mode;

	std::ifstream _file;	//Assigned file to work with
	char file_buffer_[200];	//Current line in (^) file
	char *_current_symbol;	//Current symbol in (^) line

	bool eof_;

	int _line_number;
	int _column_number;

	States _state;				//Current state of a system
	std::string _token;		//Current token
	Token return_value;
	bool return_flag;

	Codes _charIdentify(char symbol); //Type of _current_symbol

	//Processing functions

	//Utility functions for the processing functions
	void _pushAndStep();
	void _makeToken(States type);
	void _print(States type, std::string);
	void _printTable();

	//IDLE state
	void idleToIdle();
	void idleToID();
	void idleToInt();
	void idleToString();
	void idleToOperator();
	void idleToSeparator();
	void idleToError();
	void idleToComment();

	//ID state
	void IDToIdle();
	void IDToID();
	/*void IDToInt(); No cases*/
	void IDToString();	//Error state
	void IDToOperator();
	void IDToSeparator();
	void IDToError();
	void IDToComment();

	//INT state
	void intToIdle();
	void intToID();	//Error state
	void intToInt();
	void intToString();	//Error state
	void intToOperator();
	void intToSeparator();
	void intToError();
	void intToComment();

	//FLOAT state
	void floatToIdle();
	void floatToID();
	void floatToFloat();
	void floatToString();
	void floatToOperator();
	void floatToSeparator();
	void floatToError();
	void floatToComment();

	//STRING state
	void stringToIdle();
	void stringToID();
	void stringToInt();
	void stringToString();
	void stringToOperator();
	void stringToSeparator();
	void stringToError();
	void stringToComment();

	//OPERATOR state
	void operatorToIdle();
	void operatorToID();
	void operatorToInt();
	void operatorToString();
	void operatorToOperator();
	void operatorToSeparator();
	void operatorToError();
	void operatorToComment();

	//SEPARATOR state
	void separatorToIdle();
	void separatorToID();
	void separatorToInt();
	void separatorToString();
	void separatorToOperator();
	void separatorToSeparator();
	void separatorToError();
	void separatorToComment();
};
