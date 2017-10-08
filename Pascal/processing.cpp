#include "lexer.h"

using namespace std;

//Utility functions for the processing functions
void Lexer::pushAndStep()
{
	this->token_.push_back(*this->current_symbol_);
	this->current_symbol_++;
}


//IDLE state
void Lexer::idleToIdle()
{
	this->current_symbol_++;
}

void Lexer::idleToName()
{
	this->state_ = NAME;

	this->pushAndStep();
}

void Lexer::idleToOperator()
{
	this->state_ = OPERATOR;

	this->pushAndStep();
}


//NAME state
void Lexer::nameToIdle()
{
	this->state_ = IDLE;

	cout << "Lexer:: OK:: " <<  this->token_.c_str() << "  is  NAME" << endl;
	this->token_.clear();
}

void Lexer::nameToName()
{
	this->pushAndStep();
}

void Lexer::nameToOperator()
{
	this->state_ = OPERATOR;

	cout << "Lexer:: OK:: " << this->token_.c_str() << "  is  NAME" << endl;
	this->token_.clear();

	this->pushAndStep();
}


//OPERATOR state
void Lexer::operatorToIdle()
{
	this->state_ = IDLE;

	//Couple of 'if' statements should provide better operator recognition (for example: := is assignment)
	cout << "Lexer:: OK:: " << this->token_.c_str() << "  is  OPERATOR" << endl;
	this->token_.clear();
}

void Lexer::operatorToName()
{
	this->state_ = NAME;

	//Couple of 'if' statements should provide better operator recognition (for example: := is assignment)
	cout << "Lexer:: OK:: " << this->token_.c_str() << "  is  OPERATOR" << endl;
	this->token_.clear();

	this->pushAndStep();
}

void Lexer::operatorToOperator()
{
	if (this->token_.length() == 2)
	{
		cout << "Lexer:: OK:: " << this->token_.c_str() << "  is  OPERATOR" << endl;	//TODO Check this one, maybe some errors can be there
		this->token_.clear();
	}

	this->pushAndStep();
}