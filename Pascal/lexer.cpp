#include "lexer.h"

using namespace std;

Lexer::Lexer()
{
	state_ = IDLE;
	current_symbol_ = nullptr;
	token_.clear();
	line_number_ = 0;
	column_number_ = 0;

	printTable_();

	//Processing functions' pointer assignment

	//IDLE state
	action_[IDLE][SPACE] = &Lexer::idleToIdle;
	action_[IDLE][LETTER] = &Lexer::idleToName;
	action_[IDLE][DIGIT] = &Lexer::idleToInt;
	action_[IDLE][OP_SIGN] = &Lexer::idleToOperator;
	action_[IDLE][SEP_SIGN] = &Lexer::idleToSeparator;

	//NAME state
	action_[NAME][SPACE] = &Lexer::nameToIdle;
	action_[NAME][LETTER] = &Lexer::nameToName;
	action_[NAME][DIGIT] = &Lexer::nameToName;
	action_[NAME][OP_SIGN] = &Lexer::nameToOperator;
	action_[NAME][SEP_SIGN] = &Lexer::nameToSeparator;

	//INT state
	action_[INT][SPACE] = &Lexer::intToIdle;
	//action_[INT][LETTER] =
	action_[INT][DIGIT] = &Lexer::intToInt;
	action_[INT][OP_SIGN] = &Lexer::intToOperator;
	action_[INT][SEP_SIGN] = &Lexer::intToSeparator;

	//OPERATOR state
	action_[OPERATOR][SPACE] = &Lexer::operatorToIdle;
	action_[OPERATOR][LETTER] = &Lexer::operatorToName;
	action_[OPERATOR][DIGIT] = &Lexer::operatorToInt;
	action_[OPERATOR][OP_SIGN] = &Lexer::operatorToOperator;
	action_[OPERATOR][SEP_SIGN] = &Lexer::operatorToSeparator;

	//SEPARATOR state
	action_[SEPARATOR][IDLE] = &Lexer::separatorToIdle;
	action_[SEPARATOR][LETTER] = &Lexer::separatorToName;
	action_[SEPARATOR][DIGIT] = &Lexer::separatorToInt;
	action_[SEPARATOR][OP_SIGN] = &Lexer::separatorToOperator;
	action_[SEPARATOR][SEP_SIGN] = &Lexer::separatorToSeparator;
}

Lexer::~Lexer()
{
	if (file_.is_open())
		file_.close();
}

bool Lexer::fileAssign(string filename)
{
	file_.open(filename);

	if (!file_.is_open())	//Check if file is opened
	{
		cout << "FileError:: Can't open file";
		return false;
	}
		return true;
}

bool Lexer::parse()
{
	if (!file_.is_open())	//Check if we have something to work with
	{
		cout << "FileError: Can't parse - no file assigned";
		return false;
	}

	Codes sym_type;
	while (!file_.eof())	//Main parsing cycle
	{
		//Read new line of source text
		if (current_symbol_ == nullptr || *current_symbol_ == '\n' || *current_symbol_ == '\0') //TODO Check if '\0' is possible
		{
			file_.getline(file_buffer_, sizeof(char) * 200);
			current_symbol_ = file_buffer_;

			line_number_++;
			column_number_ = 0;
		}

		//Identify current symbol
		sym_type = charIdentify_(*current_symbol_);

		//Act respectively
		(this->*action_[state_][sym_type])();
	}

	while (*current_symbol_ != '\n' && *current_symbol_ != '\0')	//Continue for the whole line  /*TODO rework this bicycle*/
	{
		//Identify current symbol
		sym_type = charIdentify_(*current_symbol_);

		//Act respectively
		(this->*action_[state_][sym_type])();
	}

	//Last symbol check			/*TODO rework this bicycle*/
	sym_type = charIdentify_(*current_symbol_);
	(this->*action_[state_][sym_type])();

	return true;
}

Codes Lexer::charIdentify_(char symbol)
{
	//[' ' \n \t]
	if (symbol == ' ' || symbol == '\n' || symbol == '\t')
		return SPACE;

	//[0-9]
	if (isdigit(symbol))
		return DIGIT;

	//[A-z]
	if (isalpha(symbol))
		return LETTER;

	//[+ - * / = . := ..]
	if (symbol == '=' || symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '.')
		return OP_SIGN;

	//[, : ; () [] (. .)]
	if (symbol == ';' || symbol == ',' || symbol == '(' || symbol == ')' || symbol == ':' || symbol == '[' || symbol == ']')
		return SEP_SIGN;

	//Just to make sure we're not gonna explode on unsupported symbols
	return SPACE;
}