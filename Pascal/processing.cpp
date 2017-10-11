#include "lexer.h"

using namespace std;

//Utility functions for the processing functions
void Lexer::pushAndStep_()
{
	token_.push_back(*current_symbol_);
	current_symbol_++;
	column_number_++;
}

void Lexer::print_(string type)
{
	cout.width(8);
	cout << line_number_;
	cout.width(8);
	cout << column_number_;
	cout.width(40);
	cout << type;
	cout.width(40);
	cout << token_.c_str() << endl;

	token_.clear();
}

void Lexer::printTable_()
{
	cout.width(8);
	cout << "Line";
	cout.width(8);
	cout << "Col";
	cout.width(40);
	cout << "Type";
	cout.width(40);
	cout << "Code representation" << endl;
}


//IDLE state
void Lexer::idleToIdle()
{
	current_symbol_++;
}

void Lexer::idleToName()
{
	state_ = NAME;

	pushAndStep_();
}

void Lexer::idleToInt()
{
	state_ = INT;

	pushAndStep_();
}

void Lexer::idleToOperator()
{
	state_ = OPERATOR;

	pushAndStep_();
}

void Lexer::idleToSeparator()
{
	state_ = SEPARATOR;

	pushAndStep_();
}


//NAME state
void Lexer::nameToIdle()
{
	state_ = IDLE;

	print_("NAME");
}

void Lexer::nameToName()
{
	pushAndStep_();
}

void Lexer::nameToOperator()
{
	state_ = OPERATOR;

	print_("NAME");

	pushAndStep_();
}

void Lexer::nameToSeparator()
{
	state_ = SEPARATOR;

	print_("NAME");

	pushAndStep_();
}


//INT state
void Lexer::intToIdle()
{
	state_ = IDLE;

	print_("INT");
}

void Lexer::intToInt()
{
	pushAndStep_();
}

void Lexer::intToName()
{
	if (*current_symbol_ == 'e')
	{
		state_ = FLOAT;
		pushAndStep_();
	}
	else
	{
		state_ = IDLE;

		while (charIdentify_(*current_symbol_) == LETTER)
			pushAndStep_();
		print_("ERROR: Unexp. sym. in INT token");
	}
}

void Lexer::intToOperator()
{
	if (*current_symbol_ == '.')
	{
		state_ = FLOAT;
		pushAndStep_();
	}
	else
	{

		state_ = OPERATOR;
		print_("INT");
		pushAndStep_();
	}
}

void Lexer::intToSeparator()
{
	state_ = SEPARATOR;

	print_("INT");

	pushAndStep_();
}


//FLOAT state
void Lexer::floatToIdle()
{
	state_ = IDLE;

	print_("FLOAT");
	pushAndStep_();
}

void Lexer::floatToName()
{
	state_ = IDLE;

	while (charIdentify_(*current_symbol_) == LETTER)
		pushAndStep_();
	print_("ERROR: Unexp. sym. in FLOAT token");
}

void Lexer::floatToFloat()
{
	pushAndStep_();
}

void Lexer::floatToOperator()
{
	state_ = OPERATOR;
	print_("FLOAT");
	pushAndStep_();
}

void Lexer::floatToSeparator()
{
	state_ = SEPARATOR;
	print_("FLOAT");
	pushAndStep_();
}

//OPERATOR state
void Lexer::operatorToIdle()
{
	state_ = IDLE;

	print_("OPERATOR");
}

void Lexer::operatorToName()
{
	state_ = NAME;

	print_("OPERATOR");

	pushAndStep_();
}

void Lexer::operatorToInt()
{
	state_ = INT;

	print_("OPERATOR");

	pushAndStep_();
}

void Lexer::operatorToOperator()
{
	if (token_.length() == 2)
	{
		print_("OPERATOR");
	}

	pushAndStep_();
}

void Lexer::operatorToSeparator()	
{
	state_ = SEPARATOR;

	print_("OPERATOR");

	pushAndStep_();
}


//SEPARATOR state
void Lexer::separatorToIdle()
{
	state_ = IDLE;

	print_("SEPARATOR");

	pushAndStep_();
}

void Lexer::separatorToName()
{
	state_ = NAME;

	print_("SEPARATOR");

	pushAndStep_();
}

void Lexer::separatorToInt()
{
	state_ = INT;

	print_("SEPARATOR");

	pushAndStep_();
}

void Lexer::separatorToOperator()
{
	if ((token_ == "(" && *current_symbol_ == '.') || (token_ == ":" && *current_symbol_ == '='))
	{
		state_ = IDLE;
		pushAndStep_();
		print_("OPERATOR");
	}
	else
	{
		state_ = OPERATOR;
		print_("SEPARATOR");
		pushAndStep_();
	}
}

void Lexer::separatorToSeparator()
{
	state_ = SEPARATOR;
	print_("SEPARATOR");
	pushAndStep_();
}