#include "lexer.h"

using namespace std;

Lexer::Lexer()
{
	this->state_ = IDLE;
	this->current_symbol_ = nullptr;
	this->token_.clear();

	//Processing functions' pointer assignment

	//IDLE state
	this->action_[IDLE][SPACE] = &Lexer::idleToIdle;
	this->action_[IDLE][LETTER] = &Lexer::idleToName;
	//this->action_[IDLE][DIGIT] = 
	this->action_[IDLE][OP_SIGN] = &Lexer::idleToOperator;

	//NAME state
	this->action_[NAME][SPACE] = &Lexer::nameToIdle;
	this->action_[NAME][LETTER] = &Lexer::nameToName;
	this->action_[NAME][DIGIT] = &Lexer::nameToName;
	this->action_[NAME][OP_SIGN] = &Lexer::nameToOperator;

	//OPERATOR state
	this->action_[OPERATOR][SPACE] = &Lexer::operatorToIdle;
	this->action_[OPERATOR][LETTER] = &Lexer::operatorToName;
	//this->action_[OPERATOR][DIGIT] =
	this->action_[OPERATOR][OPERATOR] = &Lexer::operatorToOperator;
}

Lexer::~Lexer()
{
	if (this->file_.is_open())
		this->file_.close();
}

bool Lexer::fileAssign(char *filename)
{
	this->file_.open(filename);

	if (this->file_.is_open())	//Check if file is opened
	{
		cout << "Lexer:: OK:: File assigned succesfully (" << filename << ")" << endl;
		return true;
	}
	else
		return false;
}

bool Lexer::parse()
{
	if (!this->file_.is_open())	//Check if we have something to work with
	{
		cout << "Lexer:: Error:: Can't parse - no file assigned";
		return false;
	}

	int sym_type;
	while (!this->file_.eof())	//Main parsing cycle
	{
		//Read new line of source text
		if (this->current_symbol_ == nullptr || *this->current_symbol_ == '\n' || *this->current_symbol_ == '\0') //TODO Check if '\0' is possible
		{
			this->file_.getline(this->file_buffer_, sizeof(char) * 200);
			this->current_symbol_ = this->file_buffer_;
		}

		//Identify current symbol
		sym_type = this->charIdentify_(*this->current_symbol_);

		//Act respectively
		(this->*action_[this->state_][sym_type])();
	}

	while (*this->current_symbol_ != '\n' && *this->current_symbol_ != '\0')	//Continue for the whole line  /*TODO rework this bicycle*/
	{
		//Identify current symbol
		sym_type = this->charIdentify_(*this->current_symbol_);

		//Act respectively
		(this->*action_[this->state_][sym_type])();
	}

	//Last symbol check			/*TODO rework this bicycle*/
	sym_type = this->charIdentify_(*this->current_symbol_);
	(this->*action_[this->state_][sym_type])();

	cout << "Lexer:: OK:: End of file reached. Parse operation stopped" << endl;
	return true;
}

int Lexer::charIdentify_(char symbol)
{
	if (symbol == ' ' || symbol == '\n' || symbol == '\t')
		return SPACE;
	if (isdigit(symbol))
		return DIGIT;
	if (isalpha(symbol))
		return LETTER;

	//[+ - * / = :]
	//TODO Not sure about [:]
	if (symbol == ';' || symbol == ':' || symbol == '=' || symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
		return OPERATOR;

	//Just to make sure we're not gonna explode on unsupported symbols
	return SPACE;
}