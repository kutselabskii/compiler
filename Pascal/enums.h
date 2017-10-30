#pragma once
#include <string>
#include <set>
#include <map>

enum States	//State codes
{
	IDLE = 0,
	ID,
	INT,
	FLOAT,
	STRING,
	OPERATOR,
	SEPARATOR,
	ERROR,
	COMMENT,
	KEYWORD,
	ENDOFFILE,
	DATATYPE
};

const std::string stateToString[] = {
	"IDLE",
	"ID",
	"INT",
	"FLOAT",
	"STRING",
	"OPERATOR",
	"SEPARATOR",
	"ERROR",
	"COMMENT",
	"KEYWORD",
	"ENDOFFILE",
	"DATATYPE"
};

enum Codes	//Symbol codes
{
	SPACE,
	LETTER,
	DIGIT,
	OP_SIGN,
	SEP_SIGN,
	QUOTE,
	UNKNOWN,
	COM_SIGN
};

const std::set <std::string> keywords =
{ "and", "array", "asm", "begin", "break", "case", "const", "constructor", "continue", "destructor",
"div", "do", "downto", "else", "end", "false", "file", "for", "function", "goto", "if", "implementation",
"in", "inline", "interface", "label", "mod", "nil", "not", "object", "of", "on", "operator", "or", "packed",
"procedure", "program", "record", "repeat", "set", "shl", "shr", "string", "then", "to", "true", "type",
"unit", "until", "uses", "var", "while", "with", "xor"
};

const std::set <std::string> datatypes =
{
	"byte", "shortint", "smallint", "word", "integer", "cardinal", "longint", "longword", "int64", "qword",
	"real", "single", "double", "extended", "comp", "currency", "boolean", "bytebool", "wordbool", "longbool"
};

const std::set <std::string> operators =
{
	"+", "-", "*", "/", "div", "mod", "not", "and", "or", "xor", "shl", "shr", "<<", ">>", "<>", "><", "<=", ">=",
	"in", "=", "<", ">", ".", "..", ":=", "+=", "-=", "*=", "/=", ">>=", "<<="
};
