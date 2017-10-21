#include "parser.h"
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
	
	cur = _lexer->next();
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

	cur.token = "BLOCK";

	node = new Block(cur);

	node->child.push_back(parseBlockDeclarationPart());
	//node->child.push_back(parseBlockStatementPart()); Not yet
}

Node* Parser::parseBlockDeclarationPart()
{
	Node *node;
	Token cur;

	cur.token = "BLOCK DECLARATION";
	node = new Block(cur);

	cur = _lexer->next();
	if (cur.type == KEYWORD)
	{
		if (cur.token == "const")
		{
			node->child.push_back(parseConstantDefinitionPart());
		}
	}
	else
		throw ParserError(_lexer->getLine(), _lexer->getColumn(), cur.token, KEYWORDEXPECTED);
	return node;
}

Node* Parser::parseConstantDefinitionPart()
{
	Node *node;
	Token cur;

	node = parseConstantDefinition();
	cur = _lexer->next();	//or current
	while (cur.type == ID)
	{

	}
}

Node* Parser::parseConstantDefinition()
{
	Node *node;
	Token cur;

	cur = _lexer->next();
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
			case FLOAT:
				node->child.push_back(new FloatConstant(cur));
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

Node* Parser::parseExpression()
{
	Node *node;
	Token cur;

	node = parseTerm();
	cur = _lexer->current(); 
	while (cur.type == OPERATOR && (cur.token == "+" || cur.token == "-"))
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
	while (cur.type == OPERATOR && (cur.token == "*" || cur.token == "/"))
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

