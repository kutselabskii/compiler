#include "lexer.h"

using namespace std;

//Utility functions for the processing functions
void Lexer::pushAndStep_()
{
	if (current_symbol_ == nullptr || *current_symbol_ == '\n' || *current_symbol_ == '\0')
	{
		if (file_.eof())
			eof_ = true;
		else
			file_.getline(file_buffer_, sizeof(char) * 200);
		current_symbol_ = file_buffer_;
		line_number_++;
		column_number_ = 0;
	}
	else
	{
		token_.push_back(*current_symbol_);
		current_symbol_++;
		column_number_++;
	}
}

void Lexer::print_(string type)
{
	cout.width(6);
	cout << line_number_;
	cout.width(6);
	cout << column_number_;
	cout.width(30);
	if (type == "NAME" && keywords.find(token_) != keywords.end())
		cout << "KEYWORD";
	else
		cout << type;
	string h, buf;
	for (int i = 0; i < token_.length(); i++)
	{
		/*if (token_[i] == '#' && isdigit(token_[i+1]))
		{
			i++;
			while (isdigit(token_[i]))
			{
				buf.push_back(token_[i]);
				i++;
			}
			h.push_back((char)stoi(buf));
			buf.clear();
		}
		else*/
			h.push_back(tolower((unsigned char)token_[i]));
	}
	cout.width(35);
	cout << h;
	cout.width(35);
	cout << token_.c_str() << endl;
	token_.clear();

	return_value.token = h;
	return_value.type = type;
	return_flag = true;
}

void Lexer::printTable_()
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
	pushAndStep_();
}

void Lexer::idleToName()
{
	state_ = NAME;
	token_.clear();
	pushAndStep_();
}

void Lexer::idleToInt()
{
	state_ = INT;
	token_.clear();
	pushAndStep_();
}

void Lexer::idleToString()
{
	state_ = STRING;
	token_.clear();
	pushAndStep_();
}

void Lexer::idleToOperator()
{
	state_ = OPERATOR;
	token_.clear();
	pushAndStep_();
}

void Lexer::idleToSeparator()
{
	state_ = SEPARATOR;
	token_.clear();
	pushAndStep_();
}

void Lexer::idleToError()
{
	if (*current_symbol_ == '#')
	{
		state_ = INT;
		pushAndStep_();
	}
	else
	{
		while (charIdentify_(*current_symbol_) == UNKNOWN)
			pushAndStep_();
		state_ = IDLE;
		print_("ERROR: Unexp. symbol");
	}
}

void Lexer::idleToComment()
{
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
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

void Lexer::nameToString()
{
	while (charIdentify_(*current_symbol_) == LETTER || charIdentify_(*current_symbol_) == QUOTE)
		pushAndStep_();

	state_ = IDLE;
	print_("ERROR: Unexp. sym. in NAME token");
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

void Lexer::nameToError()
{
	print_("NAME");
	while (charIdentify_(*current_symbol_) == UNKNOWN)
		pushAndStep_();
	state_ = IDLE;
	print_("ERROR: Unexp. symbol");
}

void Lexer::nameToComment()
{
	print_("NAME");
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
	state_ = IDLE;
}


//INT state
void Lexer::intToIdle()
{
	state_ = IDLE;
	print_("INT");
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

void Lexer::intToInt()
{
	pushAndStep_();
}

void Lexer::intToString()
{
	while (charIdentify_(*current_symbol_) == DIGIT || charIdentify_(*current_symbol_) == QUOTE)
		pushAndStep_();

	state_ = IDLE;
	print_("ERROR: Unexp. sym. in INT token");
}

void Lexer::intToOperator()
{
	if (*current_symbol_ == '.')
	{
		if (*(current_symbol_ + 1) == '.')
		{
			state_ = OPERATOR;
			print_("INT");
			pushAndStep_();
		}
		else
		{
			state_ = FLOAT;
			pushAndStep_();
		}
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

void Lexer::intToError()
{
	print_("INT");
	while (charIdentify_(*current_symbol_) == UNKNOWN)
		pushAndStep_();
	state_ = IDLE;
	print_("ERROR: Unexp. symbol");
}


void Lexer::intToComment()
{
	print_("INT");
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
	state_ = IDLE;
}

//FLOAT state
void Lexer::floatToIdle()
{
	if (charIdentify_(token_.back()) != DIGIT)
	{
		state_ = IDLE;
		print_("ERROR: Unexp. end of a FLOAT token");
		pushAndStep_();
	}
	else
	{
		state_ = IDLE;
		print_("FLOAT");
		pushAndStep_();
	}
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

void Lexer::floatToString()
{
	while (charIdentify_(*current_symbol_) == DIGIT || charIdentify_(*current_symbol_) == QUOTE)
		pushAndStep_();

	state_ = IDLE;
	print_("ERROR: Unexp. sym. in FLOAT token");
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

void Lexer::floatToError()
{
	print_("FLOAT");
	while (charIdentify_(*current_symbol_) == UNKNOWN)
		pushAndStep_();
	state_ = IDLE;
	print_("ERROR: Unexp. symbol");
}

void Lexer::floatToComment()
{
	print_("FLOAT");
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
	state_ = IDLE;
}


//STRING state
void Lexer::stringToIdle()
{
	if (*current_symbol_ == ' ' || *current_symbol_ == '\t')
		pushAndStep_();
	else
	{
		print_("ERROR: Unexp. line end. in STRING token");
		state_ = IDLE;
	}
}

void Lexer::stringToName()
{
	pushAndStep_();
}

void Lexer::stringToInt()
{
	pushAndStep_();
}

void Lexer::stringToString()
{
	state_ = IDLE;
	pushAndStep_();
	print_("STRING");
}

void Lexer::stringToOperator()
{
	pushAndStep_();
}

void Lexer::stringToSeparator()
{
	pushAndStep_();
}

void Lexer::stringToError()
{
	pushAndStep_();
}

void Lexer::stringToComment()
{
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

void Lexer::operatorToString()
{
	state_ = STRING;
	print_("OPERATOR");
	pushAndStep_();
}

void Lexer::operatorToOperator()
{
	if (token_ == "/" && *current_symbol_ == '/')
	{
		while (*current_symbol_ != '\0' && *current_symbol_ != '\n' && !eof_)
			pushAndStep_();
		pushAndStep_();
		token_.clear();
		state_ = IDLE;
	}
	else
	{
		if (token_.length() == 2)
			if (token_ != "<<" && token_ != ">>")
				print_("OPERATOR");
		pushAndStep_();
	}
}

void Lexer::operatorToSeparator()	
{
	state_ = SEPARATOR;
	print_("OPERATOR");
	pushAndStep_();
}

void Lexer::operatorToError()
{
	print_("OPERATOR");
	while (charIdentify_(*current_symbol_) == UNKNOWN)
		pushAndStep_();
	state_ = IDLE;
	print_("ERROR: Unexp. symbol");
}

void Lexer::operatorToComment()
{
	print_("OPERATOR");
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
	state_ = IDLE;
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

void Lexer::separatorToString()
{
	state_ = STRING;
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
	else if (token_ == "(" && *current_symbol_ == '*')
	{
		while (!(*current_symbol_ == '*' && *(current_symbol_+1) == ')') && !eof_)
			pushAndStep_();
		pushAndStep_();
		pushAndStep_();
		token_.clear();
		state_ = IDLE;
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

void Lexer::separatorToError()
{
	print_("SEPARATOR");
	while (charIdentify_(*current_symbol_) == UNKNOWN)
		pushAndStep_();
	state_ = IDLE;
	print_("ERROR: Unexp. symbol");
}

void Lexer::separatorToComment()
{
	print_("SEPARATOR");
	while (*current_symbol_ != '}' && !eof_)
		pushAndStep_();
	pushAndStep_();
	token_.clear();
	state_ = IDLE;
}