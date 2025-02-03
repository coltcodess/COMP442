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
	bool match(Token* token); 
	void nextToken();

	Lexer& m_lexer;
	Grammar* m_grammar;

	// Reference to tokens 
	Token* m_lookAheadToken;
	Token* m_consumedToken;

	std::string m_sourceFileName;

	std::ofstream* m_derivationFile;
	std::ofstream* m_syntaxErrorsFile;
};

