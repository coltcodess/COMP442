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
		*m_syntaxErrorsFile << "Syntax error at line: " + std::to_string(m_lookAheadToken->position) + " expected - " + lexem;
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

bool Parser::skipErrors(std::vector<std::string> first, std::vector<std::string> follow)
{

	if(std::find(first.begin(), first.end(), m_lookAheadToken->lexem) != first.end() || 
		(std::find(first.begin(), first.end(), "EPSILON") != first.end() &&
			std::find(follow.begin(), follow.end(), m_lookAheadToken->lexem) != follow.end()))
	{
		return true;
	}
	else
	{
		*m_syntaxErrorsFile << "Syntax error at line:  " + std::to_string(m_lookAheadToken->position) << std::endl;

		while (std::find(first.begin(), first.end(), m_lookAheadToken->lexem) != first.end() ||
			std::find(follow.begin(), follow.end(), m_lookAheadToken->lexem) != follow.end())
		{
			nextToken();
			if (std::find(first.begin(), first.end(), "EPSILON") != first.end() &&
				std::find(follow.begin(), follow.end(), m_lookAheadToken->lexem) != follow.end())
			{
				return false;
			}
		}

		return true;
	}

}

bool Parser::startsymbol()
{
	std::vector<std::string> _first = { "function", "constructor", "EPSILON", "class", "implementation"};
	std::vector<std::string> _follow = {"$"};

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" || 
		m_lookAheadToken->lexem == "class" || m_lookAheadToken->lexem == "implementation")
	{
		if (prog())
		{
			*m_derivationFile << "startsymbol -> prog\n"; 
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::prog()
{
	std::vector<std::string> _first = { "function", "constructor", "EPSILON", "class", "implementation" };
	std::vector<std::string> _follow = { "$" };

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" ||
		m_lookAheadToken->lexem == "class" || m_lookAheadToken->lexem == "implementation")
	{
		if (classOrImplOrFunc())
		{
			*m_derivationFile << "prog -> classOrImplOrFunc\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::classOrImplOrFunc()
{
	nextToken();
	return true;



}

bool Parser::assignOp()
{
	std::vector<std::string> _first = {":="};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (_first[0] == m_lookAheadToken->lexem)
	{
		if (match(":="))
		{
			*m_derivationFile << "assignOp -> :=";
			
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::sign()
{
	std::vector<std::string> _first = { "+", "-"};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (_first[0] == m_lookAheadToken->lexem)
	{
		if (match("+"))
		{
			*m_derivationFile << "sign -> +\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match("-"))
		{
			*m_derivationFile << "sign -> -\n";

			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::multOp()
{
	std::vector<std::string> _first = { "and", "/", "*"};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (_first[0] == m_lookAheadToken->lexem)
	{
		if (match("and"))
		{
			*m_derivationFile << "sign -> and\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match("/"))
		{
			*m_derivationFile << "sign -> /\n";

			return true;
		}
		else return false;
	}
	else if (_first[2] == m_lookAheadToken->lexem)
	{
		if (match("*"))
		{
			*m_derivationFile << "sign -> *\n";

			return true;
		}
		else return false;
	}

	else return false;

}
