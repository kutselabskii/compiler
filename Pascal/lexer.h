#pragma once

//System includes
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>

enum States	//State codes
{
	IDLE,
	NAME,
	INT,
	FLOAT,
	STRING,
	OPERATOR,
	SEPARATOR,
	ERROR
};


enum Codes	//Symbol codes
{
	SPACE,
	LETTER,
	DIGIT,
	OP_SIGN,
	SEP_SIGN,
	QUOTE,
	UNKNOWN
};

const States fsm[100][100] = 
{
			       //Symbol
	//State          space + \n,   letter,   digit,    quote,      operator,    separator   unknown
	/*IDLE*/         {IDLE,          NAME,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR},
	/*NAME*/         {IDLE,          NAME,    NAME,     ERROR,     OPERATOR,   SEPARATOR,   ERROR},
	/*INT*/          {IDLE,         ERROR,     INT,     ERROR,     OPERATOR,       ERROR,   ERROR},
	/*FLOAT*/        {IDLE,         ERROR,   FLOAT,     ERROR,     OPERATOR,       ERROR,   ERROR},
	/*STRING*/       {STRING,      STRING,  STRING,      IDLE,       STRING,      STRING,  STRING},
	/*OPERATOR*/     {IDLE,          NAME,     INT,    STRING,        ERROR,   SEPARATOR,   ERROR},
	/*SEPARATOR*/	 {IDLE,          NAME,     INT,    STRING,     OPERATOR,   SEPARATOR,   ERROR},

	/*ERROR*/        { }
};

typedef void (*FuncType)();

class Lexer
{
public:
	Lexer();
	~Lexer();

	bool fileAssign(std::string filename);	//Assign a file to lexer (logs)
	bool parse();						//Parse assigned file (logs)

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

	//NAME state
	void nameToIdle();
	void nameToName();
	/*void nameToInt(); No cases*/
	void nameToString();	//Error state
	void nameToOperator();
	void nameToSeparator();
	void nameToError();

	//INT state
	void intToIdle();
	void intToName();	//Error state
	void intToInt();
	void intToString();	//Error state
	void intToOperator();
	void intToSeparator();
	void intToError();

	//FLOAT state
	void floatToIdle();
	void floatToName();
	void floatToFloat();
	void floatToString();
	void floatToOperator();
	void floatToSeparator();
	void floatToError();

	//STRING state
	void stringToIdle();
	void stringToName();
	void stringToInt();
	void stringToString();
	void stringToOperator();
	void stringToSeparator();
	void stringToError();

	//OPERATOR state
	void operatorToIdle();
	void operatorToName();
	void operatorToInt();
	void operatorToString();
	void operatorToOperator();
	void operatorToSeparator();
	void operatorToError();

	//SEPARATOR state
	void separatorToIdle();
	void separatorToName();
	void separatorToInt();
	void separatorToString();
	void separatorToOperator();
	void separatorToSeparator();
	void separatorToError();
};
