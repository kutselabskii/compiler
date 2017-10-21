#define _CRT_SECURE_NO_WARNINGS

//System includes
#include <iostream>
#include <string>
using namespace std;

//Project includes
#include "lexer.h"
#include "parser.h"
#include "Exceptions.h"

int main(int argc, char* argv[])
{
	string filename, mode, pause = "n";

	switch (argc)
	{
	case 1:
		cout << "Pascal compiler version 0.1 'Lexer'" << endl << "Made by Egor Kutselabskii, FEFU" << endl;
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

	if (mode != "normal" && mode != "lex" && mode != "syn")
	{
		cout << "ModeError:: incorrent mode" << endl;
		return -1;
	}

	Lexer lexer(mode);
	if (!lexer.fileAssign(filename))
	{
		if (pause == "p")
			system("pause");
		return -1;
	}

	if (mode == "normal" || mode == "syn")
	{
		Parser Parser(&lexer, mode);
		try
		{
			Parser.parse();
		}
		catch (LexerError e)
		{
			cout << e.what() << endl;
		}
		catch (ParserError e)
		{
			cout << e.what() << endl;
		}
		Parser.print();
	}
	else
		if (mode == "lex")
		{
			try
			{
				while (lexer.next().type != ENDOFFILE);
			}
			catch (LexerError e)
			{
				cout << e.what() << endl;
			}
		}


	if (pause == "p")
		system("pause");
	return 0;
}