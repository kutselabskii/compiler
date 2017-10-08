#pragma once

//System includes
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>

//Table of states
#define IDLE 0
#define NAME 1
#define INT 2
#define OPERATOR 3
#define ERROR 4

//Symbol codes
#define SPACE 0
#define LETTER 1
#define DIGIT 2
#define OP_SIGN 3


const int fsm[100][100] = 
{
			       //Symbol
	//State          space + \n,   letter,   digit,   operator (;, +, :=)
	/*IDLE*/         {IDLE,          NAME,     INT,    OPERATOR },
	/*NAME*/         {IDLE,          NAME,    NAME,    OPERATOR },
	/*INT*/          {IDLE,         ERROR,     INT,    OPERATOR },
	/*OPERATOR*/     {IDLE,          NAME,     INT,    ERROR},

	/*ERROR*/        { }
};

typedef void (*FuncType)();

class Lexer
{
public:
	Lexer();
	~Lexer();

	bool fileAssign(char *filename);	//Assign a file to lexer (logs)
	bool parse();						//Parse assigned file (logs)

private:
	void (Lexer::*action_[100][100])();	//Array of function pointers (logs sometimes)

	std::ifstream file_;	//Assigned file to work with
	char file_buffer_[200];	//Current line in (^) file
	char *current_symbol_;	//Current symbol in (^) line

	int state_;				//Current state of a system
	std::string token_;		//Current token

	int charIdentify_(char symbol); //Type of current_symbol_

	//Processing functions

	//Utility functions for the processing functions
	void pushAndStep();

	//IDLE state
	void idleToIdle();
	void idleToName();
	void idleToOperator();

	//NAME state
	void nameToIdle();
	void nameToName();
	void nameToOperator();

	//OPERATOR state
	void operatorToIdle();
	void operatorToName();
	void operatorToOperator();
};
