#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"
#include "Lexer.h"
#include "Grammar.h"

struct DerivationData
{
	std::string LHS;
	std::string RHS;

	std::string toString()
	{
		return LHS + "\t" + "->" + "\t" + RHS;
	}
};

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
	//Grammar* m_grammar;

	// Reference to tokens 
	Token* m_lookAheadToken = nullptr;
	Token* m_consumedToken = nullptr;

	std::string m_sourceFileName;

	std::ofstream* m_derivationFile;
	std::ofstream* m_syntaxErrorsFile;

	bool skipErrors(std::vector<std::string> first, std::vector<std::string> follow);

	// Non Terminals
	bool startsymbol();
	bool prog();
	bool classOrImplOrFunc();
	bool funcDef();
	bool implDef();
	bool classDecl();
	bool reptClassDecl_1();
	bool visibility();
	bool memberDecl();
	bool funcDec1();
	bool funcHead();
	bool attributeDec1();
	bool returnType();
	bool indice();



	bool fParams();
	bool fParamsTail();

	bool aParams();
	bool aParamsTail();

	bool arithExpr();
	bool arraySize();
	bool arraySize1();

	// Terminals
	bool assignOp();
	bool sign();
	bool multOp();
	bool addOp();
	bool relOp();
	bool type();





};

