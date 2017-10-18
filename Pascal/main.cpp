#define _CRT_SECURE_NO_WARNINGS

//System includes
#include <iostream>
#include <string>
using namespace std;

//Project includes
#include "lexer.h"
#include "syntaxer.h"

int main(int argc, char* argv[])
{
	string filename, mode, pause;

	switch (argc)
	{
	case 1:
		cout << "Pascal compiler version 0.1 'Lexer'" << endl << "Made by Egor Kutselabskii, FEFU" << endl << "Curator: Alexander Klenin" << endl;
		system("pause");
		return 0;
	case 2:
		filename = argv[1];
		mode = "normal";
		break;
	case 3:
		filename = argv[1];
		mode = argv[2];
		break;
	case 4:
		filename = argv[1];
		mode = argv[2];
		pause = argv[3];
	}

	if (mode != "normal" && mode != "lex")
	{
		cout << "ModeError:: incorrent mode" << endl;
		return -1;
	}

	Lexer lexer(mode);
	if (!lexer.fileAssign(filename)) //Assign our file to lexer
	{
		if (pause == "p")
			system("pause");
		return -1;
	}

	if (mode == "normal")
	{
		Syntaxer syntaxer(&lexer, mode);
		//try
		syntaxer.parse();
		//catch
		syntaxer.print();
	}
	else
	if (mode == "lex")
		while(lexer.next().type != "eof");

	//if (pause == "p")
		system("pause");
	return 0;
}