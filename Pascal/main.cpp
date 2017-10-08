#define _CRT_SECURE_NO_WARNINGS

//System includes
#include <iostream>
using namespace std;

//Project includes
#include "lexer.h"

int main(int argc, char* argv[])
{
	char filename[100];

	if (argc == 1)	//Acquiring name of file to compile
	{
		cout << "Enter filename to compile" << endl;
		cin >> filename;
	}
	else
		strcpy(filename, argv[1]);

	cout << "Reading from \"" << filename << "\"" << endl;

	Lexer lexer;
	lexer.fileAssign(filename); //Assign our file to lexer

	if (!lexer.parse())				//Parse our file
		cout << "Somewhat error happened";

	system("pause");
	return 0;
}