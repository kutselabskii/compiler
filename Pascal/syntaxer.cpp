#include "syntaxer.h"
using namespace std;

Syntaxer::Syntaxer(Lexer *lexer, string mode)
{
	_lexer = lexer;
	_mode = mode;
	_root = nullptr;
}

Syntaxer::~Syntaxer()
{

}

void Syntaxer::parse()
{
	_root = parseExpression();
}

void Syntaxer::print()
{
	printTree(_root, "", false);
}

void Syntaxer::printTree(Node *node, string prefix, bool isleft)
{
	if (node)
	{
		cout << prefix << (isleft ? "|-- " : "|__ ") << "" << node->token.token << "" << endl;
		prefix = prefix + (isleft ? "|   " : "    ");
		printTree(node->left, prefix, true);
		printTree(node->right, prefix, false);
	}
}

Node* Syntaxer::parseExpression()
{
	Node *node;
	Token cur;

	node = parseTerm();
	cur = _lexer->current(); 
	while (cur.type == "OPERATOR" && (cur.token == "+" || cur.token == "-"))
	{
		node->parent = new BinaryOperation(cur);
		node->parent->left = node;
		node = node->parent;
		node->right = parseTerm();
		cur = _lexer->current();
	}
	return node;
	
}

Node* Syntaxer::parseTerm()
{
	Node *node;
	Token cur;

	node = parseFactor();
	cur = _lexer->next();
	while (cur.type == "OPERATOR" && (cur.token == "*" || cur.token == "/"))
	{
		node->parent = new BinaryOperation(cur);
		node->parent->left = node;
		node = node->parent;
		node->right = parseFactor();
		cur = _lexer->next();
	}
	return node;
}

Node* Syntaxer::parseFactor()
{
	Node *node;
	Token cur;

	cur = _lexer->next();
	if (cur.type == "NAME")
		return new Variable(cur);
	else if (cur.type == "INT" || cur.type == "FLOAT")
		return new Constant(cur);
	else if (cur.type == "SEPARATOR" && cur.token == "(")
	{
		node = parseExpression();
		cur = _lexer->current();
		if (cur.token != ")")
		{
			cout << "ERRORRRRR" << endl;
			return nullptr;
		}
		return node;
	}
	else
	{
		cout << "ERRORRRRR" << endl;
		return nullptr;	///asdasd
	}

}

