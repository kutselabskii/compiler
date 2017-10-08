#include "lexer.h"

using namespace std;

Lexer::Lexer()
{

}

Lexer::~Lexer()
{
	if (this->file_.is_open())
		this->file_.close();
}

bool Lexer::fileAssign(char *filename)
{
	this->file_.open(filename);

	if (this->file_.is_open())
	{
		cout << "Lexer:: File assigned succesfully (" << filename << ")" << endl;
		return true;
	}
	else
		return false;
}