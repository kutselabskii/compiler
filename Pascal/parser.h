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

	//Program
	Node* parseProgram();
	Node* parseProgramHeading();

	//Everything
	Node* parseBlock();
	Node* parseBlockDeclarationPart();
	Node* parseBlockStatementPart();


	Node* parseStatement();
	Node* parseSimpleStatement();
	Node* parseStructuredStatement();

	//Simple
	Node* parseAssignmentStatement();

	//Structured
	Node* parseWhileStatement();
	Node* parseForStatement();
	Node* parseRepeatStatement();
	Node* parseIfStatement();
	Node* parseCompoundStatement();

	//Type
	Node* parseTypeDefinitionPart();
	Node* parseTypeDefinition();

	//Const
	Node* parseConstantDefinitionPart();
	Node* parseConstantDefinition();

	//Var
	Node* parseVariableDefinitionPart();
	Node* parseVariableDefinition();

	//Variable types
	Node* parseType();
	Node* parseArray();
	Node* parseRecord();

	//Expression
	Node* parseExpression();
	Node* parseSimpleExpression();
	Node* parseTerm();
	Node* parseFactor();

	//void deleteTree(Node*);

	
private:
	Lexer *_lexer;
	std::string _mode;

	Node *_root;
};