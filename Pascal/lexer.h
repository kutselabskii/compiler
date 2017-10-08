#pragma once

//System includes
#include <iostream>
#include <fstream>

//Table of states
#define IDLE 0
#define NAME 1


const int fsm[100][100] = 
{
			//Symbol
	//State   space   letter
	/*IDLE*/  {IDLE,   NAME},
	/*NAME*/  {IDLE,   NAME}
};


class Lexer
{
public:
	Lexer();
	~Lexer();

	bool fileAssign(char *filename);	//Assign a file to lexer (logs)

private:
	std::ifstream file_;
};
