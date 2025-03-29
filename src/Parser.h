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
	Node* getASTroot();

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
	bool ISA1(Node* root);
	bool ISA2(Node* root);

	bool visibility();
	bool memberDecl(Node* root);
	bool funcDec1(Node* root);
	bool funcHead(Node* root);
	bool funcBody(Node* root);
	bool LOCALVARDECLORSTAT(Node* root);
	bool LOCALVARDECLORSTAT2(Node* root);
	bool localVarDecl(Node* root);
	bool statement(Node* root);
	bool FUNCALLORASSIGN(Node* root);
	bool FUNCALLORASSIGN2(Node& child, Node* root);
	bool FUNCALLORASSIGN3(Node& child, Node* root);
	bool FUNCALLORASSIGN4(Node& child, Node* root);
	bool variable(Node* root);
	bool variable2(Node* root);
	bool reptVariable(Node& child, Node* root);
	bool varIdNest(Node* root);
	bool varIdNest2(Node* root);

	bool expr(Node* root);
	bool expr2(Node* root);
	bool arithExpr(Node* root);
	bool RIGHTRECARITHEXPR(Node& child, Node* root);
	bool term(Node& root);
	bool factor( Node* root);
	bool factor2(Node* root);

	bool indice(Node* root);

	bool rightRecTerm(Node& child, Node* root);

	bool relExpr(Node* root);
	bool statBlock(Node* root);
	bool STATEMENTS(Node* root);

	bool attributeDecl(Node* root);
	bool varDecl(Node* node);
	bool returnType(Node* root);
	bool indices(Node* root);


	bool REPTVARIABLEORFUNCTIONCALL(Node* root);

	bool idnest(Node* root);
	bool idnest2(Node* root);
	

	bool fParams(Node* root);
	bool REPTFPARAMS1(Node* root);
	bool fParamsTail(Node* root);

	bool aParams(Node* root);
	bool REPTAPARAMS1(Node* root);
	bool aParamsTail(Node* root);

	bool arraySize(Node* root);
	bool arraySize2(Node* root);

	bool arraySizes(Node* root);

	// Terminals
	bool assignOp(Node* root);
	bool sign();
	bool multOp(Node* node);
	bool IDORSELF(Node* root);
	bool addOp();
	bool relOp();
	bool type(Node* root);
	
	// Write AST to file
	void writeAST(Node* root, int level);

	// Helper 
	bool lexemInFollowSet(std::vector<std::string> _follow);

	bool tokenInFollowSet(std::vector<TokenType> _follow);
	bool tokenInFirstSet(std::vector<TokenType> _first);


};

