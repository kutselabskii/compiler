#define _CRT_SECURE_NO_WARNINGS

//System includes
#include <iostream>
#include <string>
using namespace std;

//Project includes
#include "lexer.h"

//TODO << >> comments

int main(int argc, char* argv[])
{
	string filename;

	if (argc == 1)	//Acquiring name of file to compile
	{
		cout << "Pascal compiler version 0.1 'Lexer'" << endl << "Made by Egor Kutselabskii, FEFU" << endl << "Curator: Alexander Klenin";
	}
	else
		filename = argv[1];

	Lexer lexer;
	if (!lexer.fileAssign(filename)) //Assign our file to lexer
		return -1;

	if (!lexer.parse())				//Parse our file
		cout << "Somewhat error happened";

	system("pause");
	return 0;
}

//NOW WE'RE DOING
//Float and Errors, strings