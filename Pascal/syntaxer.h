#pragma once
#include "lexer.h"
#include "tree_classes.h"

class Syntaxer
{
public:
	Syntaxer(Lexer *lexer, std::string mode);
	~Syntaxer();

	void parse();
	void print();

	void printTree(Node*, std::string, bool);

	Node* parseExpression();
	Node* parseTerm();
	Node* parseFactor();

	//void deleteTree(Node*);

	
private:
	Lexer *_lexer;
	std::string _mode;

	Node *_root;
};