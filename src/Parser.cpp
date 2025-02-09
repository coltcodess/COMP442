#include "Parser.h"

Parser::Parser(const std::string fileName, Lexer& lexer) : m_sourceFileName(fileName), m_lexer(lexer)
{
	std::cout << "Create Parser..." << std::endl;

	std::ofstream outDev(m_sourceFileName + ".outderivation", std::ofstream::out);
	std::ofstream outErrors(m_sourceFileName + ".outsyntaxerrors", std::ofstream::out);

	m_derivationFile = &outDev;
	m_syntaxErrorsFile = &outErrors;

	if (this->parse())
	{
		std::cout << "-------------- Successful Parse" << std::endl;
	}
	else
	{
		std::cout << "-------------- Failed to Parse" << std::endl; 
	}

	// Close Files 
	m_derivationFile->close();
	m_syntaxErrorsFile->close();

	m_derivationFile = nullptr;
	m_syntaxErrorsFile = nullptr;
	
}

Parser::~Parser()
{
}

bool Parser::match(std::string lexem)
{
	if (m_lookAheadToken->lexem == lexem)
	{
		nextToken();
		return true;
	}
	else
	{
		nextToken();
		return false;
	}
}

bool Parser::parse()
{
	nextToken();
	if (startsymbol() && match("$")) return true;
	else return false;

}

void Parser::nextToken()
{
	m_consumedToken = m_lookAheadToken;
	

	m_lookAheadToken = m_lexer.getNextToken();
}

bool Parser::startsymbol()
{
	// Test
	if (assignOp())
	{

		return true;
	}
}

bool Parser::assignOp()
{
	std::vector<std::string> _first = {":="};

	// Check First 
	if (std::find(_first.begin(), _first.end(), m_lookAheadToken->lexem) != _first.end())
	{
		if (match(":="))
		{
			*m_derivationFile << "assignOp -> :=";
			return true;
		}
		else return false;
	}

	else
	{
		return false;
	}
}
