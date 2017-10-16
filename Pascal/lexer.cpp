#include "lexer.h"

using namespace std;

Lexer::Lexer()
{
	state_ = IDLE;
	current_symbol_ = nullptr;
	eof_ = false;
	token_.clear();
	line_number_ = 1;
	column_number_ = 0;

	printTable_();

	//Processing functions' pointer assignment

	//IDLE state
	action_[IDLE][SPACE] = &Lexer::idleToIdle;
	action_[IDLE][LETTER] = &Lexer::idleToName;
	action_[IDLE][DIGIT] = &Lexer::idleToInt;
	action_[IDLE][QUOTE] = &Lexer::idleToString;
	action_[IDLE][OP_SIGN] = &Lexer::idleToOperator;
	action_[IDLE][SEP_SIGN] = &Lexer::idleToSeparator;
	action_[IDLE][UNKNOWN] = &Lexer::idleToError;
	action_[IDLE][COM_SIGN] = &Lexer::idleToComment;

	//NAME state
	action_[NAME][SPACE] = &Lexer::nameToIdle;
	action_[NAME][LETTER] = &Lexer::nameToName;
	action_[NAME][DIGIT] = &Lexer::nameToName;
	action_[NAME][QUOTE] = &Lexer::nameToString;
	action_[NAME][OP_SIGN] = &Lexer::nameToOperator;
	action_[NAME][SEP_SIGN] = &Lexer::nameToSeparator;
	action_[NAME][UNKNOWN] = &Lexer::nameToError;
	action_[NAME][COM_SIGN] = &Lexer::nameToComment;

	//INT state
	action_[INT][SPACE] = &Lexer::intToIdle;
	action_[INT][LETTER] = &Lexer::intToName;
	action_[INT][DIGIT] = &Lexer::intToInt;
	action_[INT][QUOTE] = &Lexer::intToString;
	action_[INT][OP_SIGN] = &Lexer::intToOperator;
	action_[INT][SEP_SIGN] = &Lexer::intToSeparator;
	action_[INT][UNKNOWN] = &Lexer::intToError;
	action_[INT][COM_SIGN] = &Lexer::intToComment;

	//FLOAT state
	action_[FLOAT][SPACE] = &Lexer::floatToIdle;
	action_[FLOAT][LETTER] = &Lexer::floatToName;
	action_[FLOAT][DIGIT] = &Lexer::floatToFloat;
	action_[FLOAT][QUOTE] = &Lexer::floatToString;
	action_[FLOAT][OP_SIGN] = &Lexer::floatToOperator;
	action_[FLOAT][SEP_SIGN] = &Lexer::floatToSeparator;
	action_[FLOAT][UNKNOWN] = &Lexer::floatToError;
	action_[FLOAT][COM_SIGN] = &Lexer::floatToComment;

	//STRING state
	action_[STRING][SPACE] = &Lexer::stringToIdle;
	action_[STRING][LETTER] = &Lexer::stringToName;
	action_[STRING][DIGIT] = &Lexer::stringToInt;
	action_[STRING][QUOTE] = &Lexer::stringToString;
	action_[STRING][OP_SIGN] = &Lexer::stringToOperator;
	action_[STRING][SEP_SIGN] = &Lexer::stringToSeparator;
	action_[STRING][UNKNOWN] = &Lexer::stringToError;
	action_[STRING][COM_SIGN] = &Lexer::stringToComment;

	//OPERATOR state
	action_[OPERATOR][SPACE] = &Lexer::operatorToIdle;
	action_[OPERATOR][LETTER] = &Lexer::operatorToName;
	action_[OPERATOR][DIGIT] = &Lexer::operatorToInt;
	action_[OPERATOR][QUOTE] = &Lexer::operatorToString;
	action_[OPERATOR][OP_SIGN] = &Lexer::operatorToOperator;
	action_[OPERATOR][SEP_SIGN] = &Lexer::operatorToSeparator;
	action_[OPERATOR][UNKNOWN] = &Lexer::operatorToError;
	action_[OPERATOR][COM_SIGN] = &Lexer::operatorToComment;

	//SEPARATOR state
	action_[SEPARATOR][IDLE] = &Lexer::separatorToIdle;
	action_[SEPARATOR][LETTER] = &Lexer::separatorToName;
	action_[SEPARATOR][DIGIT] = &Lexer::separatorToInt;
	action_[SEPARATOR][QUOTE] = &Lexer::separatorToString;
	action_[SEPARATOR][OP_SIGN] = &Lexer::separatorToOperator;
	action_[SEPARATOR][SEP_SIGN] = &Lexer::separatorToSeparator;
	action_[SEPARATOR][UNKNOWN] = &Lexer::separatorToError;
	action_[SEPARATOR][COM_SIGN] = &Lexer::separatorToComment;
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
	if (current_symbol_ == nullptr || *current_symbol_ == '\n' || *current_symbol_ == '\0')
	{
		if (file_.eof())
			eof_ = true;
		else
			file_.getline(file_buffer_, sizeof(char) * 200);
		current_symbol_ = file_buffer_;
	}

	while (!eof_)	//Main parsing cycle
	{
		//Identify current symbol
		sym_type = charIdentify_(*current_symbol_);

		//Act respectively
		(this->*action_[state_][sym_type])();
	}
	return true;
}

Codes Lexer::charIdentify_(char symbol)
{
	//[' ' \n \t]
	if (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\0')
		return SPACE;

	//[0-9]
	if (isdigit(symbol))
		return DIGIT;

	//[A-z]
	if (isalpha(symbol))
		return LETTER;

	//[+ - * / = . := .. < >]
	if (symbol == '=' || symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '.' || symbol == '<' || symbol == '>')
		return OP_SIGN;

	//[, : ; () [] (. .)]
	if (symbol == ';' || symbol == ',' || symbol == '(' || symbol == ')' || symbol == ':' || symbol == '[' || symbol == ']')
		return SEP_SIGN;

	//[']
	if (symbol == '\'')
		return QUOTE;

	if (symbol == '{')
		return COM_SIGN;

	return UNKNOWN;
}