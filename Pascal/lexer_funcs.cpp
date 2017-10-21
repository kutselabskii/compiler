#include "lexer.h"

using namespace std;

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

void Lexer::_makeToken(States type)
{
	string h, buf;

	//TODO COMPLETE
	//make #123 and $abc
	for (unsigned int i = 0; i < _token.length(); i++)	//That is ok
		h.push_back(tolower((unsigned char)_token[i]));

	if (type == OPERATOR && operators.find(_token) == operators.end())
		throw LexerError(_line_number, _column_number, *(_current_symbol - 1));

	if (type == ID && operators.find(_token) != operators.end())	//For div and mod checking
		type = OPERATOR;
	else 
		if (type == ID && keywords.find(_token) != keywords.end())
			type = KEYWORD;
	//TODO COMPLETE

	if (_mode == "lex")
	{
		_print(type, h);
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

void Lexer::_print(States type, string value)
{
	cout.width(6);
	cout << _line_number;
	cout.width(6);
	cout << _column_number;
	cout.width(30);
	cout << stateToString[type];
	cout.width(35);
	cout << value;
	cout.width(35);
	cout << _token.c_str() << endl;
}


//IDLE state
void Lexer::idleToIdle()
{
	_pushAndStep();
}

void Lexer::idleToID()
{
	_state = ID;
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
		throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::idleToComment()
{
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
}


//ID state
void Lexer::IDToIdle()
{
	_state = IDLE;
	_makeToken(ID);
}

void Lexer::IDToID()
{
	_pushAndStep();
}

void Lexer::IDToString()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::IDToOperator()
{
	_state = OPERATOR;
	_makeToken(ID);
	_pushAndStep();
}

void Lexer::IDToSeparator()
{
	_state = SEPARATOR;
	_makeToken(ID);
	_pushAndStep();
}

void Lexer::IDToError()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::IDToComment()
{
	_makeToken(ID);
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
	_makeToken(INT);
}

void Lexer::intToID()
{
	if (*_current_symbol == 'e')
	{
		_state = FLOAT;
		_pushAndStep();
	}
	else
		throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::intToInt()
{
	_pushAndStep();
}

void Lexer::intToString()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::intToOperator()
{
	if (*_current_symbol == '.')
	{
		if (*(_current_symbol + 1) == '.')
		{
			_state = OPERATOR;
			_makeToken(INT);
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
		_makeToken(INT);
		_pushAndStep();
	}
}

void Lexer::intToSeparator()
{
	_state = SEPARATOR;
	_makeToken(INT);
	_pushAndStep();
}

void Lexer::intToError()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}


void Lexer::intToComment()
{
	_makeToken(INT);
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
		throw LexerError(_line_number, _column_number, *_current_symbol);
	}
	else
	{
		_state = IDLE;
		_makeToken(FLOAT);
		_pushAndStep();
	}
}

void Lexer::floatToID()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::floatToFloat()
{
	_pushAndStep();
}

void Lexer::floatToString()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::floatToOperator()
{
	_state = OPERATOR;
	_makeToken(FLOAT);
	_pushAndStep();
}

void Lexer::floatToSeparator()
{
	_state = SEPARATOR;
	_makeToken(FLOAT);
	_pushAndStep();
}

void Lexer::floatToError()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::floatToComment()
{
	_makeToken(FLOAT);
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
		throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::stringToID()
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
	_makeToken(STRING);
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
	_makeToken(OPERATOR);
}

void Lexer::operatorToID()
{
	_state = ID;
	_makeToken(OPERATOR);
	_pushAndStep();
}

void Lexer::operatorToInt()
{
	_state = INT;
	_makeToken(OPERATOR);
	_pushAndStep();
}

void Lexer::operatorToString()
{
	_state = STRING;
	_makeToken(OPERATOR);
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
				_makeToken(OPERATOR);
		_pushAndStep();
	}
}

void Lexer::operatorToSeparator()	
{
	_state = SEPARATOR;
	_makeToken(OPERATOR);
	_pushAndStep();
}

void Lexer::operatorToError()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::operatorToComment()
{
	_makeToken(OPERATOR);
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
	_makeToken(SEPARATOR);
	_pushAndStep();
}

void Lexer::separatorToID()
{
	_state = ID;
	_makeToken(SEPARATOR);
	_pushAndStep();
}

void Lexer::separatorToInt()
{
	_state = INT;
	_makeToken(SEPARATOR);
	_pushAndStep();
}

void Lexer::separatorToString()
{
	_state = STRING;
	_makeToken(SEPARATOR);
	_pushAndStep();
}

void Lexer::separatorToOperator()
{
	if ((_token == "(" && *_current_symbol == '.') || (_token == ":" && *_current_symbol == '='))
	{
		_state = IDLE;
		_pushAndStep();
		_makeToken(OPERATOR);
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
		_makeToken(SEPARATOR);
		_pushAndStep();
	}
}

void Lexer::separatorToSeparator()
{
	_state = SEPARATOR;
	_makeToken(SEPARATOR);
	_pushAndStep();
}

void Lexer::separatorToError()
{
	throw LexerError(_line_number, _column_number, *_current_symbol);
}

void Lexer::separatorToComment()
{
	_makeToken(SEPARATOR);
	while (*_current_symbol != '}' && !eof_)
		_pushAndStep();
	_pushAndStep();
	_token.clear();
	_state = IDLE;
}