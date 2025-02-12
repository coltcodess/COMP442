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

bool Parser::match(TokenType type)
{
	if (m_lookAheadToken->type == type)
	{
		nextToken();
		return true;
	}
	else
	{
		*m_syntaxErrorsFile << "Syntax error at line: " + std::to_string(m_lookAheadToken->position) + " | expected - " + std::to_string(type) + " | received - " + m_lookAheadToken->convertTokenTypeToString() + "\n";

		nextToken();
		return false;
	}
}

bool Parser::parse()
{
	nextToken();
	if (startsymbol() && match(TokenType::END_OF_FILE)) return true;
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
		*m_syntaxErrorsFile << "Syntax error at line:  " + std::to_string(m_lookAheadToken->position) + m_lookAheadToken->lexem << std::endl;

		while (!(std::find(first.begin(), first.end(), m_lookAheadToken->lexem) != first.end() ||
			std::find(follow.begin(), follow.end(), m_lookAheadToken->lexem) != follow.end()))
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

/*
	Non - Terminal



*/

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
	std::vector<std::string> _first = { "function", "constructor", "EPSILON", "class", "implementation" };
	std::vector<std::string> _follow = { "$" };

	if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" || m_lookAheadToken->lexem == "EPSILON")
	{
		if (funcDef())
		{
			*m_derivationFile << "classOrImplOrFunc -> funcDef\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "class")
	{
		
		if (classDecl())
		{
			*m_derivationFile << "classOrImplOrFunc -> classDecl\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "implementation")
	{
		if (implDef())
		{
			*m_derivationFile << "classOrImplOrFunc -> implDef\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::funcDef()
{
	return false;
}

bool Parser::implDef()
{
	return false;
}

bool Parser::classDecl()
{
	std::vector<std::string> _first = {"class"};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == "class")
	{
		if (match(TokenType::CLASS) && match(TokenType::id) && match(TokenType::OPENCUBR) && 
			reptClassDecl_1() && match(TokenType::CLOSECUBR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "classDecl -> class id { reptclassDecl_1 };\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::reptClassDecl_1()
{
	std::vector<std::string> _first = { "private", "public", "function", "constructor", "attribute", "reptclassDec_1", "EPSILON"};
	std::vector<std::string> _follow = {"}"};

	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "private" || m_lookAheadToken->lexem == "public" || 
		m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" || 
		m_lookAheadToken->lexem == "attribute")
	{
		if (visibility() && reptClassDecl_1())
		{
			*m_derivationFile << "reptclassDecl_1 -> visibility\n";
			*m_derivationFile << "reptclassDecl_1 -> memberDec1\n";
			return true;
		}
		else return false;
	}
	// Contains EPILSON
	else if (m_lookAheadToken->lexem == "}")
	{
		*m_derivationFile << "reptclassDecl_1 -> EPSILON\n";
		return true;
	}
	else return false;
	
}

bool Parser::visibility()
{
	std::vector<std::string> _first = {"private", "public", "EPSILON"};
	std::vector<std::string> _follow = { "function", "constructor", "attribute", "reptclassDecl_1"};

	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "private")
	{
		if (match(TokenType::PRIVATE))
		{
			*m_derivationFile << "visibility -> 'private'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "public")
	{
		if (match(TokenType::PUBLIC))
		{
			*m_derivationFile << "visibility -> 'public'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" ||
		m_lookAheadToken->lexem == "attribute" || m_lookAheadToken->lexem == "reptclassDecl_1")
	{
		*m_derivationFile << "visibility -> 'EPSILON'\n";
		return true;
	}
	else return false;
	
}

/*
	Terminal 



*/


bool Parser::assignOp()
{
	std::vector<std::string> _first = {":="};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	// Check First 
	if (_first[0] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::ASSIGN))
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
		if (match(TokenType::PLUS))
		{
			*m_derivationFile << "sign -> +\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::MINUS))
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
		if (match(TokenType::AND))
		{
			*m_derivationFile << "sign -> and\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::DIV))
		{
			*m_derivationFile << "sign -> /\n";

			return true;
		}
		else return false;
	}
	else if (_first[2] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::MULTI))
		{
			*m_derivationFile << "sign -> *\n";

			return true;
		}
		else return false;
	}

	else return false;

}


