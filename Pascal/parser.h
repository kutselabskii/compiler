#pragma once
#include "lexer.h"
#include "tree_classes.h"

class Parser
{
public:
	Parser(Lexer *lexer, std::string mode);
	~Parser();

	void parse();
	void print();

	void printTree(Node*, std::string, bool);


	Node* parseProgram();
	Node* parseProgramHeading();

	Node* parseBlock();
	Node* parseBlockDeclarationPart();
	Node* parseBlockStatementPart();

	Node* parseConstantDefinitionPart();
	Node* parseConstantDefinition();


	Node* parseExpression();
	Node* parseTerm();
	Node* parseFactor();

	//void deleteTree(Node*);

	
private:
	Lexer *_lexer;
	std::string _mode;

	Node *_root;
};