#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"
#include "Lexer.h"

/// Assignment 3
#include "NodeFactory.h"

class Parser
{

public:
	Parser(const std::string fileName, Lexer& lexer);
	~Parser();
	bool parse();

private:
	bool match(TokenType type);
	bool nextToken();


	Lexer& m_lexer;
	NodeFactory* m_nodeFactory;
	Node* m_astRoot = nullptr;

	// Reference to tokens 
	Token* m_lookAheadToken = nullptr;
	Token* m_consumedToken = nullptr;

	std::string m_sourceFileName;

	std::ofstream* m_derivationFile;
	std::ofstream* m_syntaxErrorsFile;
	std::ofstream* m_ASTFile;

	std::ofstream* astFile;
	bool skipErrors(bool containsESPILON, std::vector<TokenType> first, std::vector<TokenType> follow);

	// Non Terminals
	bool startsymbol();
	bool prog();
	bool classOrImplOrFunc(Node* root);
	bool funcDef(Node* root);
	bool implDef(Node* root);
	bool impleBody(Node* root);
	bool classDecl(Node* root);
	bool VISMEMBERDECL(Node* root);
	bool ISA1();
	bool ISA2();

	bool visibility();
	bool memberDecl(Node* root);
	bool funcDec1(Node* root);
	bool funcHead(Node* root);
	bool funcBody(Node* root);
	bool LOCALVARDECLORSTAT();
	bool LOCALVARDECLORSTAT2();
	bool localVarDecl();
	bool statement();
	bool FUNCALLORASSIGN();
	bool FUNCALLORASSIGN2();
	bool FUNCALLORASSIGN3();
	bool FUNCALLORASSIGN4();
	bool variable();
	bool variable2();
	bool reptVariable();
	bool varIdNest();
	bool varIdNest2();

	bool expr();
	bool expr2();
	bool arithExpr();
	bool RIGHTRECARITHEXPR();
	bool term();
	bool factor();
	bool factor2();

	bool indice();

	bool rightRecTerm();

	bool relExpr();
	bool statBlock();
	bool STATEMENTS();

	bool attributeDecl();
	bool varDecl();
	bool returnType();
	bool indices();


	bool REPTVARIABLEORFUNCTIONCALL();

	bool idnest();
	bool idnest2();
	

	bool fParams();
	bool REPTFPARAMS1();
	bool fParamsTail();

	bool aParams();
	bool REPTAPARAMS1();
	bool aParamsTail();

	bool arraySize();
	bool arraySize2();

	bool arraySizes();

	// Terminals
	bool assignOp();
	bool sign();
	bool multOp();
	bool IDORSELF();
	bool addOp();
	bool relOp();
	bool type();
	
	// Write AST to file
	void writeAST(Node* root, int level);

	// Helper 
	bool lexemInFollowSet(std::vector<std::string> _follow);

	bool tokenInFollowSet(std::vector<TokenType> _follow);
	bool tokenInFirstSet(std::vector<TokenType> _first);

};

