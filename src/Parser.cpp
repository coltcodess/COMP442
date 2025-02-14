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
	if(!nextToken()) return false;
	if (startsymbol()) return true;
	else return false;

}

bool Parser::nextToken()
{

	m_consumedToken = m_lookAheadToken;
	m_lookAheadToken = m_lexer.getNextToken();

	// Check if look ahead token is nullptr, meaning we are done parsing tokens. 
	if (m_lookAheadToken == nullptr)
	{
		std::cout << "Error: Parser ran out of tokens!";
		return false; 
	}

	return true;
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
			if(!nextToken()) return false;
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
	else if (m_lookAheadToken->type == TokenType::END_OF_FILE)
	{
		if (match(TokenType::END_OF_FILE))
		{
			*m_derivationFile << "startsymbol -> $\n";
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
		if (classOrImplOrFunc() && prog())
		{
			*m_derivationFile << "prog -> classOrImplOrFunc prog\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::END_OF_FILE)
	{
		if (match(TokenType::END_OF_FILE))
		{
			*m_derivationFile << "prog -> $\n";
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

	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor")
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
	else if (m_lookAheadToken->type == TokenType::END_OF_FILE)
	{
		if (match(TokenType::END_OF_FILE))
		{
			*m_derivationFile << "classOrImplOrFunc -> EPSILON\n";
			return true;
		}
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
			*m_derivationFile << "classDecl -> 'class' 'id' '{' reptClassDecl_1 '}' ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::reptClassDecl_1()
{
	std::vector<std::string> _first = { "private", "public", "function", "constructor", "attribute", "EPSILON"};
	std::vector<std::string> _follow = {"}"};

	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "private" || m_lookAheadToken->lexem == "public" || 
		m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor" || 
		m_lookAheadToken->lexem == "attribute")
	{
		if (visibility() && memberDecl() && reptClassDecl_1())
		{
			*m_derivationFile << "reptclassDecl_1 -> visibility memberDec1 reptClassDecl_1\n";
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
	std::vector<std::string> _follow = { "function", "constructor", "attribute"};

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
		m_lookAheadToken->lexem == "attribute")
	{
		*m_derivationFile << "visibility -> 'EPSILON'\n";
		return true;
	}
	else return false;
	
}

bool Parser::memberDecl()
{
	std::vector<std::string> _first = { "function", "constructor", "attribute", "EPSILON" };
	std::vector<std::string> _follow = { "}" };

	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "constructor" || m_lookAheadToken->lexem == "function")
	{
		if (funcDec1())
		{
			*m_derivationFile << "memberDecl -> funcDec1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "attribute")
	{
		if (attributeDec1())
		{
			*m_derivationFile << "memberDecl -> attributeDec1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "}")
	{
		*m_derivationFile << "memberDecl -> 'EPSILON'\n";
		return true;
	}
	
	else return false;
}

bool Parser::funcDec1()
{
	std::vector<std::string> _first = {"function", "constructor"};
	std::vector<std::string> _follow = {};
	
	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "constructor" || m_lookAheadToken->lexem == "function")
	{
		if (funcHead() && match(TokenType::SEMI))
		{
			*m_derivationFile << "funcDec1 -> funcHead\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::funcHead()
{
	std::vector<std::string> _first = { "function", "constructor" };
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow));

	if (m_lookAheadToken->lexem == "constructor")
	{
		if (match(TokenType::CONSTRUCTOR) && match(TokenType::OPENPAR) && fParams() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "funcHead -> 'constructor' '(' fParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "function")
	{
		if (match(TokenType::FUNCTION) && match(TokenType::id) && match(TokenType::OPENPAR) && fParams() && 
			match(TokenType::CLOSEPAR) && match(TokenType::ARROW) && returnType())
		{
			*m_derivationFile << "'function' 'id' '(' fParams ')' '=>' returnType\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::attributeDec1()
{
	std::vector<std::string> _first = { "attribute"};
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "attribute")
	{
		if (match(TokenType::ATTRIBUTE) && varDec1())
		{
			*m_derivationFile << "attributeDec1 -> 'attribute' varDec1\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::varDec1()
{
	std::vector<std::string> _first = { "id" };
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(TokenType::id) && match(TokenType::COLON) && type() && arraySize())
		{
			*m_derivationFile << "varDec1 -> 'id' ':' type arraySize\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::returnType()
{
	std::vector<std::string> _first = { "int", "float", "id", "void"};
	std::vector<std::string> _follow = {};

	// Checks lexem and not the tokentype >:(
	//if (!skipErrors(_first, _follow)) return false;
	
	if (m_lookAheadToken->type == TokenType::INT ||
		m_lookAheadToken->type == TokenType::FLOAT ||
		m_lookAheadToken->type == TokenType::id)
	{
		if (type())
		{
			*m_derivationFile << "returnType -> type\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "void")
	{
		if (match(TokenType::VOID))
		{
			*m_derivationFile << "returnType -> 'void'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::indice()
{
	std::vector<std::string> _first = { "[" };

	if (m_lookAheadToken->lexem == "[")
	{
		if (match(TokenType::OPENSQBR) && arithExpr() && match(TokenType::CLOSESQBR))
		{
			*m_derivationFile << "indice -> '[' arithExpr ']'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::arraySize()
{
	std::vector<std::string> _first = { "[" };
	std::vector<std::string> _follow = { ",", ")", ";"};

	if (m_lookAheadToken->lexem == "[")
	{
		if (match(TokenType::OPENSQBR) && arraySize1())
		{
			*m_derivationFile << "arraySize -> '[' arraySize1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ";" || m_lookAheadToken->lexem == ")" || m_lookAheadToken->lexem == ",")
	{
		*m_derivationFile << "arraySize -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::arraySize1()
{
	std::vector<std::string> _first = { "]", "intnum" };

	if (m_lookAheadToken->lexem == "]")
	{
		if (match(TokenType::CLOSESQBR))
		{
			*m_derivationFile << "arraySize1 -> ']'\n";
			return true;
		}
	}
	else if (m_lookAheadToken->type == TokenType::intnum)
	{
		if (match(TokenType::intnum) && match(TokenType::CLOSESQBR))
		{
			*m_derivationFile << "arraySize1 -> 'intnum' ']'\n";
			return true;
		}
	}
	else return false;
}

bool Parser::fParams()
{
	std::vector<std::string> _first = { "id", "EPSILON" };
	std::vector<std::string> _follow = { ")" };

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(TokenType::id) && match(TokenType::COLON) && type() && arraySize() && fParamsTail())
		{
			*m_derivationFile << "fParams -> 'id' ':' type arraySize fParamsTail\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ")")
	{
		*m_derivationFile << "fParams -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::fParamsTail()
{
	std::vector<std::string> _first = { ",", "EPSILON"};
	std::vector<std::string> _follow = { ")"};
	
	if (m_lookAheadToken->lexem == ",")
	{
		if (match(TokenType::COMMA) && match(TokenType::id) && match(TokenType::COLON) && type() && arraySize() &&
			fParamsTail())
		{
			*m_derivationFile << "fParasTail -> ',' 'id' ':' type arraySize\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ")")
	{
		*m_derivationFile << "fParamsTail -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::aParams()
{
	std::vector<std::string> _first = { ",", "floatLit", "id", "intLit", "not", "+", "-"};
	std::vector<std::string> _follow = { };

	if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->lexem == "not" || m_lookAheadToken->lexem == "+" || m_lookAheadToken->lexem == "-")
	{
		if (expr() && aParamsTail())
		{
			*m_derivationFile << "aParams -> expr aParamsTail\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::aParamsTail()
{
	std::vector<std::string> _first = { ",", "EPSILON" };
	std::vector<std::string> _follow = { ")" };

	if (m_lookAheadToken->lexem == ",")
	{
		if (match(TokenType::COMMA) && expr() && aParamsTail())
		{
			*m_derivationFile << "aParamsTail -> ',' expr aParamsTail\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ")")
	{
		if (match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "aParamsTail -> EPSILON\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::expr()
{
	return true;
}

bool Parser::arithExpr()
{
	return true;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

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

bool Parser::addOp()
{
	std::vector<std::string> _first = { "or", "+", "-" };
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == "or")
	{
		if (match(TokenType::OR))
		{
			*m_derivationFile << "addOp -> 'or'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "+")
	{
		if (match(TokenType::PLUS))
		{
			*m_derivationFile << "addOp -> '+'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "-")
	{
		if (match(TokenType::MINUS))
		{
			*m_derivationFile << "addOp -> '-'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::relOp()
{
	std::vector<std::string> _first = { ">=", ">", "==", "<=", "<", "<>", };
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == ">=")
	{
		if (match(TokenType::GEQ))
		{
			*m_derivationFile << "relOp -> '>='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ">")
	{
		if (match(TokenType::GT))
		{
			*m_derivationFile << "relOp -> '>'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "==")
	{
		if (match(TokenType::EQ))
		{
			*m_derivationFile << "relOp -> '=='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "<=")
	{
		if (match(TokenType::LEQ))
		{
			*m_derivationFile << "relOp -> '<='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "<")
	{
		if (match(TokenType::LT))
		{
			*m_derivationFile << "relOp -> '<'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "<>")
	{
		if (match(TokenType::NOTEQ))
		{
			*m_derivationFile << "relOp -> '<>'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::type()
{
	std::vector<std::string> _first = { "int", "float", "id" };
	std::vector<std::string> _follow = {};

	//if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == "int")
	{
		if (match(TokenType::INT))
		{
			*m_derivationFile << "type -> 'int'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "float")
	{
		if (match(TokenType::FLOAT))
		{
			*m_derivationFile << "type -> 'float'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(TokenType::id))
		{
			*m_derivationFile << "type -> 'id'\n";
			return true;
		}
		else return false;
	}

	else return false;
}


