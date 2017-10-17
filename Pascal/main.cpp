#define _CRT_SECURE_NO_WARNINGS

//System includes
#include <iostream>
#include <string>
using namespace std;

//Project includes
#include "lexer.h"

//TODO read #12123123 and $123123123, transform it into real values

int main(int argc, char* argv[])
{
	string filename;

	if (argc == 1)	//Acquiring name of file to compile
	{
		cout << "Pascal compiler version 0.1 'Lexer'" << endl << "Made by Egor Kutselabskii, FEFU" << endl << "Curator: Alexander Klenin" << endl;
		system("pause");
		return 0;
	}
	else
		filename = argv[1];

	Lexer lexer;
	if (!lexer.fileAssign(filename)) //Assign our file to lexer
	{
		system("pause");
		return -1;
	}

	while(lexer.next().type != "eof");

	system("pause");
	return 0;
}