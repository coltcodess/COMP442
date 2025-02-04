#include "Parser.h"

Parser::Parser(const std::string fileName, Lexer& lexer) : m_sourceFileName(fileName), m_lexer(lexer)
{
	std::cout << "Create Parser..." << std::endl;

	std::ofstream outDev(m_sourceFileName + ".outderivation", std::ofstream::out);
	std::ofstream outErrors(m_sourceFileName + ".outsyntaxerrors", std::ofstream::out);




	// Close Files 
	m_derivationFile = &outDev;
	m_syntaxErrorsFile = &outErrors;

	m_derivationFile->close();
	m_syntaxErrorsFile->close();

	m_derivationFile = nullptr;
	m_syntaxErrorsFile = nullptr;
	
}

Parser::~Parser()
{
}

bool Parser::match(Token* token)
{
	if (m_lookAheadToken == token)
	{
		m_lookAheadToken = m_lexer.getNextToken();
		return true;
	}
	else
	{
		m_lookAheadToken = m_lexer.getNextToken();
		return false;
	}
}

bool Parser::parse()
{
	//m_lookAheadToken = m_lexer.getNextToken();

	//// Create EOF token type
	//Token* token = new Token(TokenType::END_OF_FILE, "$", -1);

	//// Check start symbol and last token 
	//if (startSymbol() && match(token))
	//{
	//	free(token);
	//	token = nullptr;

	//	return true;
	//}
	//else
	//{
	//	free(token);
	//	token = nullptr;

	//	return false;
	//}


	return true;
}

void Parser::nextToken()
{
	m_consumedToken = m_lookAheadToken;
	m_lookAheadToken = m_lexer.getNextToken();
}

bool Parser::assignOp()
{
	if (m_lookAheadToken->type == TokenType::ASSIGN)
	{
		*m_derivationFile << "assignOp -> :="; 
		return true;
	}
	else
	{
		return false;
	}
}
