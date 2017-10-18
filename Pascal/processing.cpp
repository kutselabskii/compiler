#include "lexer.h"

using namespace std;

//Utility functions for the processing functions
void Lexer::_pushAndStep()
{
	if (_current_symbol == nullptr || *_current_symbol == '\n' || *_current_symbol == '\0')
	{
		if (_file.eof())
			eof_ = true;
		else
			_file.getline(file_buffer_, sizeof(char) * 200);
		_current_symbol = file_buffer_;
		_line_number++;
		_column_number = 0;
	}
	else
	{
		_token.push_back(*_current_symbol);
		_current_symbol++;
		_column_number++;
	}
}

void Lexer::_print(string type)
{
	string h, buf;
	for (int i = 0; i < _token.length(); i++)
		h.push_back(tolower((unsigned char)_token[i]));

	if (type == "NAME" && keywords.find(_token) != keywords.end())
		type = "KEYWORD";

	if (_mode == "lex")
	{
		cout.width(6);
		cout << _line_number;
		cout.width(6);
		cout << _column_number;
		cout.width(30);
		cout << type;
		cout.width(35);
		cout << h;
		cout.width(35);
		cout << _token.c_str() << endl;
	}
	_token.clear();

	return_value.token = h;
	return_value.type = type;
	return_flag = true;
}

void Lexer::_printTable()
{
	cout.width(6);
	cout << "Line";
	cout.width(6);
	cout << "Col";
	cout.width(30);
	cout << "Type";
	cout.width(35);
	cout << "Value";
	cout.width(35);
	cout << "Code representation" << endl;
}


//IDLE state
void Lexer::idleToIdle()
{
	_pushAndStep();
}

void Lexer::idleToName()
{
	_state = NAME;
	_token.clear();
	_pushAndStep();
}

void Lexer::idleToInt()
{
	_state = INT;
	_token.clear();
	_pushAndStep();
}

void Lexer::idleToString()
{
	_state = STRING;
	_token.clear();
	_pushAndStep();
}

void Lexer::idleToOperator()
{
	_state = OPERATOR;
	_token.clear();
	_pushAndStep();
}

void Lexer::idleToSeparator()
{
	_state = SEPARATOR;
	_token.clear();
	_pushAndStep();
}

void Lexer::idleToError()
{
	if (*_current_symbol == '#')
	{
		_state = INT;
		_pushAndStep();
	}
	else
	{
		while (_charIdentify(*_current_symbol) == UNKNOWN)
			_pushAndStep();
		_state = IDLE;
		_print("ERROR: Unexp. symbol");
	}
}

void Lexer::idleToComment()
{
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
}


//NAME state
void Lexer::nameToIdle()
{
	_state = IDLE;
	_print("NAME");
}

void Lexer::nameToName()
{
	_pushAndStep();
}

void Lexer::nameToString()
{
	while (_charIdentify(*_current_symbol) == LETTER || _charIdentify(*_current_symbol) == QUOTE)
		_pushAndStep();

	_state = IDLE;
	_print("ERROR: Unexp. sym. in NAME token");
}

void Lexer::nameToOperator()
{
	_state = OPERATOR;
	_print("NAME");
	_pushAndStep();
}

void Lexer::nameToSeparator()
{
	_state = SEPARATOR;
	_print("NAME");
	_pushAndStep();
}

void Lexer::nameToError()
{
	_print("NAME");
	while (_charIdentify(*_current_symbol) == UNKNOWN)
		_pushAndStep();
	_state = IDLE;
	_print("ERROR: Unexp. symbol");
}

void Lexer::nameToComment()
{
	_print("NAME");
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}


//INT state
void Lexer::intToIdle()
{
	_state = IDLE;
	_print("INT");
}

void Lexer::intToName()
{
	if (*_current_symbol == 'e')
	{
		_state = FLOAT;
		_pushAndStep();
	}
	else
	{
		_state = IDLE;

		while (_charIdentify(*_current_symbol) == LETTER)
			_pushAndStep();
		_print("ERROR: Unexp. sym. in INT token");
	}
}

void Lexer::intToInt()
{
	_pushAndStep();
}

void Lexer::intToString()
{
	while (_charIdentify(*_current_symbol) == DIGIT || _charIdentify(*_current_symbol) == QUOTE)
		_pushAndStep();

	_state = IDLE;
	_print("ERROR: Unexp. sym. in INT token");
}

void Lexer::intToOperator()
{
	if (*_current_symbol == '.')
	{
		if (*(_current_symbol + 1) == '.')
		{
			_state = OPERATOR;
			_print("INT");
			_pushAndStep();
		}
		else
		{
			_state = FLOAT;
			_pushAndStep();
		}
	}
	else
	{

		_state = OPERATOR;
		_print("INT");
		_pushAndStep();
	}
}

void Lexer::intToSeparator()
{
	_state = SEPARATOR;
	_print("INT");
	_pushAndStep();
}

void Lexer::intToError()
{
	_print("INT");
	while (_charIdentify(*_current_symbol) == UNKNOWN)
		_pushAndStep();
	_state = IDLE;
	_print("ERROR: Unexp. symbol");
}


void Lexer::intToComment()
{
	_print("INT");
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}

//FLOAT state
void Lexer::floatToIdle()
{
	if (_charIdentify(_token.back()) != DIGIT)
	{
		_state = IDLE;
		_print("ERROR: Unexp. end of a FLOAT token");
		_pushAndStep();
	}
	else
	{
		_state = IDLE;
		_print("FLOAT");
		_pushAndStep();
	}
}

void Lexer::floatToName()
{
	_state = IDLE;

	while (_charIdentify(*_current_symbol) == LETTER)
		_pushAndStep();
	_print("ERROR: Unexp. sym. in FLOAT token");
}

void Lexer::floatToFloat()
{
	_pushAndStep();
}

void Lexer::floatToString()
{
	while (_charIdentify(*_current_symbol) == DIGIT || _charIdentify(*_current_symbol) == QUOTE)
		_pushAndStep();

	_state = IDLE;
	_print("ERROR: Unexp. sym. in FLOAT token");
}

void Lexer::floatToOperator()
{
	_state = OPERATOR;
	_print("FLOAT");
	_pushAndStep();
}

void Lexer::floatToSeparator()
{
	_state = SEPARATOR;
	_print("FLOAT");
	_pushAndStep();
}

void Lexer::floatToError()
{
	_print("FLOAT");
	while (_charIdentify(*_current_symbol) == UNKNOWN)
		_pushAndStep();
	_state = IDLE;
	_print("ERROR: Unexp. symbol");
}

void Lexer::floatToComment()
{
	_print("FLOAT");
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}


//STRING state
void Lexer::stringToIdle()
{
	if (*_current_symbol == ' ' || *_current_symbol == '\t')
		_pushAndStep();
	else
	{
		_print("ERROR: Unexp. line end. in STRING token");
		_state = IDLE;
	}
}

void Lexer::stringToName()
{
	_pushAndStep();
}

void Lexer::stringToInt()
{
	_pushAndStep();
}

void Lexer::stringToString()
{
	_state = IDLE;
	_pushAndStep();
	_print("STRING");
}

void Lexer::stringToOperator()
{
	_pushAndStep();
}

void Lexer::stringToSeparator()
{
	_pushAndStep();
}

void Lexer::stringToError()
{
	_pushAndStep();
}

void Lexer::stringToComment()
{
	_pushAndStep();
}

//OPERATOR state
void Lexer::operatorToIdle()
{
	_state = IDLE;
	_print("OPERATOR");
}

void Lexer::operatorToName()
{
	_state = NAME;
	_print("OPERATOR");
	_pushAndStep();
}

void Lexer::operatorToInt()
{
	_state = INT;
	_print("OPERATOR");
	_pushAndStep();
}

void Lexer::operatorToString()
{
	_state = STRING;
	_print("OPERATOR");
	_pushAndStep();
}

void Lexer::operatorToOperator()
{
	if (_token == "/" && *_current_symbol == '/')
	{
		while (*_current_symbol != '\0' && *_current_symbol != '\n' && !eof_)
			_pushAndStep();
		_pushAndStep();
		_token.clear();
		_state = IDLE;
	}
	else
	{
		if (_token.length() == 2)
			if (_token != "<<" && _token != ">>")
				_print("OPERATOR");
		_pushAndStep();
	}
}

void Lexer::operatorToSeparator()	
{
	_state = SEPARATOR;
	_print("OPERATOR");
	_pushAndStep();
}

void Lexer::operatorToError()
{
	_print("OPERATOR");
	while (_charIdentify(*_current_symbol) == UNKNOWN)
		_pushAndStep();
	_state = IDLE;
	_print("ERROR: Unexp. symbol");
}

void Lexer::operatorToComment()
{
	_print("OPERATOR");
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}


//SEPARATOR state
void Lexer::separatorToIdle()
{
	_state = IDLE;
	_print("SEPARATOR");
	_pushAndStep();
}

void Lexer::separatorToName()
{
	_state = NAME;
	_print("SEPARATOR");
	_pushAndStep();
}

void Lexer::separatorToInt()
{
	_state = INT;
	_print("SEPARATOR");
	_pushAndStep();
}

void Lexer::separatorToString()
{
	_state = STRING;
	_print("SEPARATOR");
	_pushAndStep();
}

void Lexer::separatorToOperator()
{
	if ((_token == "(" && *_current_symbol == '.') || (_token == ":" && *_current_symbol == '='))
	{
		_state = IDLE;
		_pushAndStep();
		_print("OPERATOR");
	}
	else if (_token == "(" && *_current_symbol == '*')
	{
		while (!(*_current_symbol == '*' && *(_current_symbol+1) == ')') && !eof_)
			_pushAndStep();
		_pushAndStep();
		_pushAndStep();
		_token.clear();
		_state = IDLE;
	}
	else
	{
		_state = OPERATOR;
		_print("SEPARATOR");
		_pushAndStep();
	}
}

void Lexer::separatorToSeparator()
{
	_state = SEPARATOR;
	_print("SEPARATOR");
	_pushAndStep();
}

void Lexer::separatorToError()
{
	_print("SEPARATOR");
	while (_charIdentify(*_current_symbol) == UNKNOWN)
		_pushAndStep();
	_state = IDLE;
	_print("ERROR: Unexp. symbol");
}

void Lexer::separatorToComment()
{
	_print("SEPARATOR");
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}