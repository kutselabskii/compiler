﻿#include "parser.h"
using namespace std;

Parser::Parser(Lexer *lexer, string mode)
{
	_lexer = lexer;
	_mode = mode;
	_root = nullptr;
}

Parser::~Parser()
{

}

void Parser::parse()
{
	//_root = parseExpression();
	_root = parseProgram();
}

void Parser::print()
{
	printTree(_root, "", false);
}

void Parser::printTree(Node *node, string prefix, bool isleft)
{
	if (node)
	{
		cout << prefix << (isleft ? "|-- " : "|__ ") << "" << node->token.token << "" << endl;
		prefix = prefix + (isleft ? "|   " : "    ");

		for (unsigned int i = 1; i < node->child.size(); i++)
			printTree(node->child[i], prefix, true);
		if (!node->child.empty())
			printTree(node->child[0], prefix, false);
	}
}

Node* Parser::parseProgram()
{
	Node *node;
	Token cur;

	node = parseProgramHeading();
	node->child.push_back(parseBlock());
	
	cur = _lexer->current();
	if (cur.token != ".")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ".");

	return node;
}

Node* Parser::parseProgramHeading()
{
	Node *node;
	Token cur;

	cur = _lexer->next();
	if (cur.type == KEYWORD && cur.token == "program")
	{
		cur = _lexer->next();
		if (cur.type == ID)
			node = new ProgramHeading(cur);
		else 
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, ID);
		cur = _lexer->next();
		if (cur.token != ";")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
		return node;
	}
	throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "program");
}

Node* Parser::parseBlock()
{
	Node *node;
	Token cur;

	cur.token = "Block";

	node = new Block(cur);

	node->child.push_back(parseBlockDeclarationPart());
	node->child.push_back(parseBlockStatementPart());

	return node;
}

Node* Parser::parseBlockDeclarationPart()
{
	Node *node;
	Token cur;

	cur.token = "Block declaration";
	node = new Block(cur);

	cur = _lexer->next();
	while (cur.type == KEYWORD && cur.token != "begin" && cur.token != ".")
	{
		if (cur.token == "const")
			node->child.push_back(parseConstantDefinitionPart());
		if (cur.token == "var")
			node->child.push_back(parseVariableDefinitionPart());
		if (cur.token == "type")
			node->child.push_back(parseTypeDefinitionPart());

		cur = _lexer->current();
	}

	return node;
}

Node* Parser::parseBlockStatementPart()
{
	Node *node;
	Token cur;

	cur.token = "Block statement";
	node = new Block(cur);

	cur = _lexer->current();
	if (cur.token == ".")
		return nullptr;

	if (cur.token != "begin")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "begin");

	cur = _lexer->next();
	while (cur.token != "end")
	{
		node->child.push_back(parseStatement());
		cur = _lexer->current();

		//DIRTY HACK
		if (cur.token == ";")
			cur = _lexer->next();
		//DIRTY HACK

		if (cur.type == ENDOFFILE || cur.token == ".")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "end");
	}
	
	_lexer->next();	//Careful

	return node;
}

Node* Parser::parseStatement()
{
	Node *node;
	Token cur;

	cur = _lexer->current();

	//DIRTY HACK
	if (cur.token == ";")
		cur = _lexer->next();
	//DIRTY HACK

	if (cur.type == KEYWORD)
		node = parseStructuredStatement();
	else
		node = parseSimpleStatement();

	return node;
}

Node* Parser::parseSimpleStatement()
{
	Node *node;
	Token cur;

	cur = _lexer->current();
	if (cur.type == ID)
		node = parseAssignmentStatement();
	//ifs
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, EXPRESSIONEXPECTED);
	return node;
}

Node* Parser::parseStructuredStatement()
{
	Node *node;
	Token cur;

	cur = _lexer->current();

	if (cur.token == "while")
		node = parseWhileStatement();
	else if (cur.token == "for")
		node = parseForStatement();
	else if (cur.token == "repeat")
		node = parseRepeatStatement();
	else if (cur.token == "begin")
		node = parseCompoundStatement();
	else if (cur.token == "if")
		node = parseIfStatement();
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, EXPRESSIONEXPECTED);

	return node;
}

Node* Parser::parseCompoundStatement()
{
	Node *node;
	Token cur;

	cur.token = "Compound statement";
	node = new Statement(cur);
	cur = _lexer->next();
	while (cur.token != "end")
	{
		node->child.push_back(parseStatement());
		cur = _lexer->next();
	}
	cur = _lexer->next();
	if (cur.token != ";")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");

	return node;
}

Node* Parser::parseIfStatement()
{
	Node *node;
	Token cur;

	cur.token = "If";

	node = new Statement(cur);

	cur.token = "Condition";
	node->child.push_back(new Statement(cur));

	Node *h = node->child.back();
	h->child.push_back(parseExpression());

	cur = _lexer->current();
	if (cur.token != "then")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "then");

	cur.token = "Statement";
	node->child.push_back(new Statement(cur));

	_lexer->next();

	h = node->child.back();
	h->child.push_back(parseStatement());

	cur = _lexer->current();
	if (cur.token == ";")
	{
		_lexer->next();
		return node;
	}
	else if (cur.token == "else")
	{
		cur.token = "Otherwise";
		node->child.push_back(new Statement(cur));

		_lexer->next();

		h = node->child.back();
		h->child.push_back(parseStatement());

		return node;
	}
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
}

Node* Parser::parseForStatement()
{
	Node *node;
	Token cur;

	cur.token = "For";

	node = new Statement(cur);

	cur.token = "Parameter";
	node->child.push_back(new Statement(cur));

	cur = _lexer->next();
	if (cur.type != ID)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);

	Node *h = node->child.back();
	h->child.push_back(new Variable(cur));

	cur = _lexer->next();
	if (cur.token != ":=")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ":=");

	cur.token = "Initial expression";
	node->child.push_back(new Statement(cur));

	h = node->child.back();
	h->child.push_back(parseExpression());

	cur.token = "Cycle operation";
	node->child.push_back(new Statement(cur));

	cur = _lexer->current();
	if (cur.token != "to" && cur.token != "downto")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "to or downto");

	h = node->child.back();
	h->child.push_back(new Statement(cur));

	cur.token = "Final expression";
	node->child.push_back(new Statement(cur));

	h = node->child.back();
	h->child.push_back(parseExpression());

	cur = _lexer->current();

	if (cur.token != "do")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "do");

	_lexer->next();
	node->child.push_back(parseStatement());

	return node;
}

Node* Parser::parseWhileStatement()
{
	Node *node;
	Token cur;

	cur.token = "While";

	node = new Statement(cur);

	cur.token = "Condition";
	node->child.push_back(new Statement(cur));

	Node *h = node->child.back();
	h->child.push_back(parseExpression());

	cur = _lexer->current();

	if (cur.token != "do")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "do");

	_lexer->next();
	node->child.push_back(parseStatement());

	return node;
}

Node* Parser::parseRepeatStatement()
{
	Node *node;
	Token cur;

	cur.token = "Repeat";

	node = new Statement(cur);

	cur = _lexer->next();
	while (cur.token != "until")
	{
		node->child.push_back(parseStatement());
		cur = _lexer->next();
	}
	

	if (cur.type == ENDOFFILE)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "until");

	cur.token = "Condition";
	node->child.push_back(new Statement(cur));

	Node *h = node->child.back();
	h->child.push_back(parseExpression());

	cur = _lexer->current();

	if (cur.token != ";")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");

	return node;
}

Node* Parser::parseAssignmentStatement()
{
	Node *node;
	Token cur;

	cur.token = "Assignment";

	node = new Statement(cur);

	cur = _lexer->current();
	node->child.push_back(new Variable(cur));

	cur = _lexer->next();

	//REMAKE, REMEMBER += ETC
	//TODOTODO
	//TODOTODO
	if (cur.token != ":=")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, OPERATOREXPECTED);
	Node *child = node->child.back();
	child->child.push_back(parseExpression());
	return node;
	//TODOTODO
	//TODOTODO

}

Node* Parser::parseConstantDefinitionPart()
{
	Node *node;
	Token cur;

	cur.token = "Constant definition";
	node = new Statement(cur);

	cur = _lexer->next();
	if (cur.type != ID)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	while (cur.type == ID)
	{
		node->child.push_back(parseConstantDefinition());
		cur = _lexer->next();
		if (cur.token != ";")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
		cur = _lexer->next();
	}
	return node;
}

Node* Parser::parseConstantDefinition()
{
	Node *node;
	Token cur;

	cur = _lexer->current();
	if (cur.type == ID)
	{
		node = new Constant(cur);
		cur = _lexer->next();
		if (cur.token == "=")
		{
			cur = _lexer->next();
			switch (cur.type)
			{
			case INT:
				node->child.push_back(new IntConstant(cur));
				break;
			case FLOAT:
				node->child.push_back(new FloatConstant(cur));
				break;
			default:
				throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, CONSTANTEXPECTED);
			}
			return node;
		}
		else
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "=");
	}
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
}

Node* Parser::parseVariableDefinitionPart()
{
	Node *node;
	Token cur;

	cur.token = "Variable definition";
	node = new Statement(cur);

	cur = _lexer->next();
	if (cur.type != ID)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	while (cur.type == ID)
	{
		node->child.push_back(parseVariableDefinition());
		cur = _lexer->next();
		if (cur.token != ";")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
		cur = _lexer->next();
	}
	return node;
}

Node* Parser::parseVariableDefinition()
{
	Node *node;
	Token cur;
	
	cur.token = "";
	node = new Variable(cur);

	cur = _lexer->current();
	while (cur.type == ID)
	{
		node->child.push_back(new Variable(cur));
		cur = _lexer->next();
		if (cur.token == ",")
		{
			cur = _lexer->next();
			if (cur.type == ID)
				;
			else
				throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
		}
		else if (cur.token == ":")
		{
			Node *h = parseType();

			node->token.type = h->token.type;
			node->token.token = h->token.token;

			for (unsigned int i = 0; i < h->child.size(); i++)
				node->child.push_back(h->child[i]);

			return node;
		}
		else
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "=");
	}
}

Node* Parser::parseType()
{
	Node *node;
	Token cur;

	cur = _lexer->next();
	if (cur.type == DATATYPE)
		node = new Statement(cur);
	else if (cur.token == "array")
		node = parseArray();
	else if (cur.token == "record")
		node = parseRecord();
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, KEYWORDEXPECTED);

	return node;
}

Node* Parser::parseRecord()
{
	Node *node;
	Token cur;

	cur.token = "Record";
	node = new Statement(cur);

	cur = _lexer->next();

	if (cur.type != ID)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	while (cur.type == ID)
	{
		node->child.push_back(parseVariableDefinition());
		cur = _lexer->next();
		if (cur.token != ";")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
		cur = _lexer->next();
	}
	
	if (cur.token != "end")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "end");

	return node;
}

Node* Parser::parseArray()
{
	Node *node;
	Token cur;

	cur.token = "Array";

	node = new Node(cur);

	cur = _lexer->next();
	if (cur.token != "[")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "[");

	do
	{
		cur = _lexer->next();
		if (cur.type != INT)
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, CONSTANTEXPECTED);

		Token sample;
		sample.token = "Range";
		Node *h = new Statement(sample);

		h->child.push_back(new Constant(cur));

		cur = _lexer->next();

		if (cur.token != "..")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "..");

		cur = _lexer->next();
		if (cur.type != INT)
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, CONSTANTEXPECTED);

		h->child.push_back(new Constant(cur));

		node->child.push_back(h);

		cur = _lexer->next();
	} while (cur.token == ",");

	if (cur.token != "]")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "]");


	cur = _lexer->next();

	if (cur.token != "of")
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "of");

	Node *h;
	cur.token = "Element type";

	h = new Statement(cur);
	h->child.push_back(parseType());

	node->child.push_back(h);

	return node;
}

Node* Parser::parseTypeDefinitionPart()
{
	Node *node;
	Token cur;

	cur.token = "Type definition";
	node = new Statement(cur);

	cur = _lexer->next();
	if (cur.type != ID)
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	while (cur.type == ID)
	{
		node->child.push_back(parseTypeDefinition());
		cur = _lexer->next();
		if (cur.token != ";")
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, ";");
		cur = _lexer->next();
	}
	return node;
}

Node* Parser::parseTypeDefinition()
{
	Node *node;
	Token cur;

	cur = _lexer->current();
	if (cur.type == ID)
	{
		node = new Statement(cur);
		cur = _lexer->next();
		if (cur.token == "=")
		{
			cur = _lexer->next();
			if (cur.type == DATATYPE)
				node->child.push_back(new Statement(cur));
			else
				throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, CONSTANTEXPECTED);
			return node;
		}
		else
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, SYMBOLEXPECTED, "=");
	}
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
}

Node* Parser::parseExpression()
{
	Node *node;
	Token cur;

	node = parseSimpleExpression();
	cur = _lexer->current(); 
	while (cur.type == OPERATOR && (cur.token == "=" || cur.token == "<>" || cur.token == "<" || cur.token == ">" || cur.token == "<=" || cur.token == ">="))
	{
		node->parent = new BinaryOperation(cur);
		node->parent->child.push_back(node);
		node = node->parent;
		node->child.push_back(parseSimpleExpression());
		cur = _lexer->current();
	}
	return node;
}

Node* Parser::parseSimpleExpression()
{
	Node *node;
	Token cur;

	node = parseTerm();
	cur = _lexer->current();
	while (cur.type == OPERATOR && (cur.token == "+" || cur.token == "-" || cur.token == "or"))
	{
		node->parent = new BinaryOperation(cur);
		node->parent->child.push_back(node);
		node = node->parent;
		node->child.push_back(parseTerm());
		cur = _lexer->current();
	}
	return node;
}

Node* Parser::parseTerm()
{
	Node *node;
	Token cur;

	node = parseFactor();
	cur = _lexer->next();
	while (cur.type == OPERATOR && (cur.token == "*" || cur.token == "/" || cur.token == "div" || cur.token == "mod" || cur.token == "and"))
	{
		node->parent = new BinaryOperation(cur);
		node->parent->child.push_back(node);
		node = node->parent;
		node->child.push_back(parseFactor());
		cur = _lexer->next();
	}
	return node;
}

Node* Parser::parseFactor()
{
	Node *node;
	Token cur;

	cur = _lexer->next();
	switch (cur.type)
	{
	case ID:
		return new Variable(cur);
	case INT:
		return new IntConstant(cur);
	case FLOAT:
		return new FloatConstant(cur);
	case SEPARATOR:
		if (cur.token == "(")
		{
			node = parseExpression();
			cur = _lexer->current();
			if (cur.token != ")")
				throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, PARENTHESISEXPECTED);

			return node;
		}
		else
			throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	default:
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, IDEXPECTED);
	}
}