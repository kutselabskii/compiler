#include "lexer.h"

using namespace std;

Lexer::Lexer(string mode)
{
	_mode = mode;
	_state = IDLE;
	_current_symbol = nullptr;
	eof_ = false;
	_token.clear();
	_line_number = 1;
	_column_number = 0;
	return_flag = false;

	if (_mode == "lex")
		_printTable();

	//IDLE state
	_action[IDLE][SPACE] = &Lexer::idleToIdle;
	_action[IDLE][LETTER] = &Lexer::idleToName;
	_action[IDLE][DIGIT] = &Lexer::idleToInt;
	_action[IDLE][QUOTE] = &Lexer::idleToString;
	_action[IDLE][OP_SIGN] = &Lexer::idleToOperator;
	_action[IDLE][SEP_SIGN] = &Lexer::idleToSeparator;
	_action[IDLE][UNKNOWN] = &Lexer::idleToError;
	_action[IDLE][COM_SIGN] = &Lexer::idleToComment;

	//NAME state
	_action[NAME][SPACE] = &Lexer::nameToIdle;
	_action[NAME][LETTER] = &Lexer::nameToName;
	_action[NAME][DIGIT] = &Lexer::nameToName;
	_action[NAME][QUOTE] = &Lexer::nameToString;
	_action[NAME][OP_SIGN] = &Lexer::nameToOperator;
	_action[NAME][SEP_SIGN] = &Lexer::nameToSeparator;
	_action[NAME][UNKNOWN] = &Lexer::nameToError;
	_action[NAME][COM_SIGN] = &Lexer::nameToComment;

	//INT state
	_action[INT][SPACE] = &Lexer::intToIdle;
	_action[INT][LETTER] = &Lexer::intToName;
	_action[INT][DIGIT] = &Lexer::intToInt;
	_action[INT][QUOTE] = &Lexer::intToString;
	_action[INT][OP_SIGN] = &Lexer::intToOperator;
	_action[INT][SEP_SIGN] = &Lexer::intToSeparator;
	_action[INT][UNKNOWN] = &Lexer::intToError;
	_action[INT][COM_SIGN] = &Lexer::intToComment;

	//FLOAT state
	_action[FLOAT][SPACE] = &Lexer::floatToIdle;
	_action[FLOAT][LETTER] = &Lexer::floatToName;
	_action[FLOAT][DIGIT] = &Lexer::floatToFloat;
	_action[FLOAT][QUOTE] = &Lexer::floatToString;
	_action[FLOAT][OP_SIGN] = &Lexer::floatToOperator;
	_action[FLOAT][SEP_SIGN] = &Lexer::floatToSeparator;
	_action[FLOAT][UNKNOWN] = &Lexer::floatToError;
	_action[FLOAT][COM_SIGN] = &Lexer::floatToComment;

	//STRING state
	_action[STRING][SPACE] = &Lexer::stringToIdle;
	_action[STRING][LETTER] = &Lexer::stringToName;
	_action[STRING][DIGIT] = &Lexer::stringToInt;
	_action[STRING][QUOTE] = &Lexer::stringToString;
	_action[STRING][OP_SIGN] = &Lexer::stringToOperator;
	_action[STRING][SEP_SIGN] = &Lexer::stringToSeparator;
	_action[STRING][UNKNOWN] = &Lexer::stringToError;
	_action[STRING][COM_SIGN] = &Lexer::stringToComment;

	//OPERATOR state
	_action[OPERATOR][SPACE] = &Lexer::operatorToIdle;
	_action[OPERATOR][LETTER] = &Lexer::operatorToName;
	_action[OPERATOR][DIGIT] = &Lexer::operatorToInt;
	_action[OPERATOR][QUOTE] = &Lexer::operatorToString;
	_action[OPERATOR][OP_SIGN] = &Lexer::operatorToOperator;
	_action[OPERATOR][SEP_SIGN] = &Lexer::operatorToSeparator;
	_action[OPERATOR][UNKNOWN] = &Lexer::operatorToError;
	_action[OPERATOR][COM_SIGN] = &Lexer::operatorToComment;

	//SEPARATOR state
	_action[SEPARATOR][IDLE] = &Lexer::separatorToIdle;
	_action[SEPARATOR][LETTER] = &Lexer::separatorToName;
	_action[SEPARATOR][DIGIT] = &Lexer::separatorToInt;
	_action[SEPARATOR][QUOTE] = &Lexer::separatorToString;
	_action[SEPARATOR][OP_SIGN] = &Lexer::separatorToOperator;
	_action[SEPARATOR][SEP_SIGN] = &Lexer::separatorToSeparator;
	_action[SEPARATOR][UNKNOWN] = &Lexer::separatorToError;
	_action[SEPARATOR][COM_SIGN] = &Lexer::separatorToComment;
}

Lexer::~Lexer()
{
	if (_file.is_open())
		_file.close();
}

bool Lexer::fileAssign(string filename)
{
	_file.open(filename);

	if (!_file.is_open())
	{
		cout << "FileError:: Can't open file" << endl;
		return false;
	}

	if (_current_symbol == nullptr || *_current_symbol == '\n' || *_current_symbol == '\0')
	{
		if (_file.eof())
			eof_ = true;
		else
			_file.getline(file_buffer_, sizeof(char) * 200);
		_current_symbol = file_buffer_;
	}
	return true;
}

Token Lexer::current()
{
	return return_value;
}

Token Lexer::next()
{
	Codes sym_type;

	while (!return_flag)
	{
		if (!eof_)
		{
			sym_type = _charIdentify(*_current_symbol);
			(this->*_action[_state][sym_type])();
		}
		else
		{
			Token t;
			t.type = "eof";
			return t;
		}
	}

	return_flag = false;
	return return_value;

}

Codes Lexer::_charIdentify(char symbol)
{
	if (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\0')
		return SPACE;

	if (isdigit(symbol))
		return DIGIT;

	if (isalpha(symbol))
		return LETTER;

	if (symbol == '=' || symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '.' || symbol == '<' || symbol == '>')
		return OP_SIGN;

	if (symbol == ';' || symbol == ',' || symbol == '(' || symbol == ')' || symbol == ':' || symbol == '[' || symbol == ']')
		return SEP_SIGN;

	if (symbol == '\'')
		return QUOTE;

	if (symbol == '{')
		return COM_SIGN;

	return UNKNOWN;
}