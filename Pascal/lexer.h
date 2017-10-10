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
	SEP_SIGN
};

const States fsm[100][100] = 
{
			       //Symbol
	//State          space + \n,   letter,   digit,   operator,    separator
	/*IDLE*/         {IDLE,          NAME,     INT,    OPERATOR,   SEPARATOR},
	/*NAME*/         {IDLE,          NAME,    NAME,    OPERATOR,   SEPARATOR},
	/*INT*/          {IDLE,         ERROR,     INT,    OPERATOR,   ERROR},
	/*OPERATOR*/     {IDLE,          NAME,     INT,       ERROR,   SEPARATOR},
	/*SEPARATOR*/	 {IDLE,          NAME,     INT,    OPERATOR,   SEPARATOR},

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
	void idleToOperator();
	void idleToSeparator();

	//NAME state
	void nameToIdle();
	void nameToName();
	void nameToOperator();
	/*void nameToInt(); No cases*/
	void nameToSeparator();

	//INT state
	void intToIdle();
	/*void intToName(); No cases*/
	void intToInt();
	void intToOperator();
	void intToSeparator();

	//OPERATOR state
	void operatorToIdle();
	void operatorToName();
	void operatorToInt();
	void operatorToOperator();
	void operatorToSeparator();

	//SEPARATOR state
	void separatorToIdle();
	void separatorToName();
	void separatorToInt();
	void separatorToOperator();
	void separatorToSeparator();
};
