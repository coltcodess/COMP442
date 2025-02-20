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

	// Reference to tokens 
	Token* m_lookAheadToken = nullptr;
	Token* m_consumedToken = nullptr;

	std::string m_sourceFileName;

	std::ofstream* m_derivationFile;
	std::ofstream* m_syntaxErrorsFile;

	// Create AST outfile 
	std::ofstream* astFile;

	bool skipErrors(std::vector<std::string> first, std::vector<std::string> follow);

	// Non Terminals
	bool startsymbol();
	bool prog();
	bool classOrImplOrFunc();
	bool funcDef();
	bool reptFuncDef();
	bool implDef();
	bool classDecl();
	bool reptClassDecl_1();
	bool optClassDecl();
	bool reptOptClassDecl();

	bool visibility();
	bool memberDecl();
	bool funcDec1();
	bool funcHead();
	bool funcBody();
	bool localVarDeclOrStatRep();
	bool localVarDeclOrStat();
	bool localVarDecl();
	bool statement();
	bool assignStat();
	bool variable();
	bool variable2();
	bool reptVariable();
	bool varIdNest();
	bool varIdNest2();

	bool expr();
	bool expr2();
	bool arithExpr();
	bool arithExpr2();
	bool term();
	bool factor();
	bool repIdNest1();

	bool rightRecTerm();

	bool relExpr();
	bool statBlock();
	bool repStatBlock();

	bool attributeDec1();
	bool varDec1();
	bool returnType();
	bool indice();

	bool idnest();
	bool idnest2();
	
	bool functionCall();

	bool fParams();
	bool fParamsTail();

	bool aParams();
	bool aParamsTail();

	bool arraySize();
	bool arraySize1();

	// Terminals
	bool assignOp();
	bool sign();
	bool multOp();
	bool addOp();
	bool relOp();
	bool type();

	// Helper 
	bool lexemInFollowSet(std::vector<std::string> _follow);

	bool tokenInFollowSet(std::vector<TokenType> _follow);
	bool tokenInFirstSet(std::vector<TokenType> _first);
};

