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
	std::vector<std::string> _first = { "function", "constructor", "class", "implementation" };
	std::vector<std::string> _follow = {  };

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
	else return false;
}

bool Parser::funcDef()
{
	std::vector<std::string> _first = { "function", "constructor", "ESPILON" };
	std::vector<std::string> _follow = { "}" };

	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor")
	{
		if (funcHead() && funcBody())
		{
			*m_derivationFile << "funcDef -> funcHead funcBody\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "}")
	{
		*m_derivationFile << "funcDef -> ESPILON\n";
		return true;
	}
	else return false;
}

bool Parser::reptFuncDef()
{
	std::vector<std::string> _first = { "function", "constructor", "ESPILON" };
	std::vector<std::string> _follow = { "}" };

	if (m_lookAheadToken->lexem == "function" || m_lookAheadToken->lexem == "constructor")
	{
		if (funcHead() && funcBody() && reptFuncDef())
		{
			*m_derivationFile << "funcDef -> funcHead funcBody\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "}")
	{
		*m_derivationFile << "funcDef -> ESPILON\n";
		return true;
	}
	else return false;
}

bool Parser::implDef()
{
	std::vector<std::string> _first = { "implementation" };

	if (m_lookAheadToken->lexem == "implementation")
	{
		if (match(TokenType::IMPLEMENTATION) && match(TokenType::id) && match(TokenType::OPENCUBR) && reptFuncDef() && match(TokenType::CLOSECUBR))
		{
			*m_derivationFile << "implDef -> 'implementation' 'id' '{' funcDef '}'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::classDecl()
{
	std::vector<std::string> _first = {"class"};
	std::vector<std::string> _follow = {};

	if (!skipErrors(_first, _follow)) return false;

	if (m_lookAheadToken->lexem == "class")
	{
		if (match(TokenType::CLASS) && match(TokenType::id) && optClassDecl() && match(TokenType::OPENCUBR) &&
			reptClassDecl_1() && match(TokenType::CLOSECUBR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "classDecl -> 'class' 'id' optClassDecl() '{' reptClassDecl_1 '}' ';'\n";
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

bool Parser::optClassDecl()
{
	std::vector<std::string> _first = { "isa", "EPSILON"};
	std::vector<std::string> _follow = { "{" };

	if (m_lookAheadToken->lexem == "isa")
	{
		if (match(TokenType::ISA) && match(TokenType::id) && reptOptClassDecl())
		{
			*m_derivationFile << "optClassDecl -> 'isa' 'id' reptOptClassDecl\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "{")
	{
		*m_derivationFile << "optClassDecl -> 'isa' 'id' reptOptClassDecl\n";
		return true;
	}
	else return false;
}

bool Parser::reptOptClassDecl()
{
	std::vector<std::string> _first = { "isa", "EPSILON" };
	std::vector<std::string> _follow = { "{" };

	if (m_lookAheadToken->lexem == ",")
	{
		if (match(TokenType::COMMA) && match(TokenType::id) && reptOptClassDecl())
		{
			*m_derivationFile << "reptOptClassDecl -> ',' 'id' reptOptClassDecl\n";
			return true;
		}
	}
	else if (m_lookAheadToken->lexem == "{")
	{
		*m_derivationFile << "reptOptClassDecl -> EPSILON\n";
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

bool Parser::funcBody()
{
	std::vector<std::string> _first = { "{" };
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "{")
	{
 		if (match(TokenType::OPENCUBR) && localVarDeclOrStatRep() && match(TokenType::CLOSECUBR))
		{
			*m_derivationFile << "funcBody -> '{' localVarDeclOrStatRep '}'\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::localVarDeclOrStatRep()
{
	std::vector<std::string> _first = { "local", "if", "read", "return", "while", "write", "id", "EPSILON"};
	std::vector<std::string> _follow = { "}" };

	if (m_lookAheadToken->lexem == "if" || m_lookAheadToken->lexem == "read"
		|| m_lookAheadToken->lexem == "while" || m_lookAheadToken->lexem == "write"
		|| m_lookAheadToken->lexem == "return" || m_lookAheadToken->lexem == "local" || m_lookAheadToken->type == TokenType::id)
	{
		if (localVarDeclOrStat() && localVarDeclOrStatRep())
		{
			*m_derivationFile << "localVarDeclOrStatRep -> localVarDeclOrStat\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "}")
	{
		*m_derivationFile << "localVarDeclOrStatRep -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::localVarDeclOrStat()
{
	std::vector<std::string> _first = { "local", "if", "read", "return", "while", "write", "id"};
	std::vector<std::string> _follow = { "}" };

	if (m_lookAheadToken->lexem == "local")
	{
		if (localVarDecl())
		{
			*m_derivationFile << "localVarDeclOrStat -> localVarDecl\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "if" || m_lookAheadToken->lexem == "read"
		|| m_lookAheadToken->lexem == "while" || m_lookAheadToken->lexem == "write" || m_lookAheadToken->lexem == "return" || m_lookAheadToken->type == TokenType::id)
	{
		if (statement())
		{
			*m_derivationFile << "localVarDeclOrStat -> statement\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::localVarDecl()
{
	std::vector<std::string> _first = { "local"};
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "local")
	{
		if (match(TokenType::LOCAL) && varDec1())
		{
			*m_derivationFile << "localVarDecl -> 'local' varDecl ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::statement()
{
	std::vector<std::string> _first = {"id", "write", "while", "return", "read", "if", ".", ":="};
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "write")
	{
		if (match(TokenType::WRITE) && match(TokenType::OPENPAR) && expr() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'write' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::id)
	{
		if (assignStat() && match(SEMI))
		{
			*m_derivationFile << "statement -> assignStat ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "return")
	{
		if (match(TokenType::RETURN) && match(TokenType::OPENPAR) && expr() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'return' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "read")
	{
		if (match(TokenType::READ) && match(TokenType::OPENPAR) && reptVariableOrFunctionCall() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'read' '(' reptVariableOrFunctionCall ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "while")
	{
		if (match(TokenType::WHILE) && match(TokenType::OPENPAR) && relExpr() && match(TokenType::CLOSEPAR) && statBlock() && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'while' '(' relExpr ')' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "if")
	{
		if (match(TokenType::IF) && match(TokenType::OPENPAR) && relExpr() && match(TokenType::THEN) && statBlock() && 
			match(TokenType::ELSE) && statBlock() && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'if' '(' relExpr ')' 'then' statBlock 'else' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->lexem == "." || m_lookAheadToken->lexem == ":=")
	{
		if (assignStat() && match(SEMI))
		{
			*m_derivationFile << "statement -> assignStat ';'\n";
			return true;
		}
	}
	else return false;
}

bool Parser::assignStat()
{
	std::vector<std::string> _first = { "id"};

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (variable() && assignOp() && expr())
		{
			*m_derivationFile << "assignStat -> variable assignOp expr\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::variable()
{
	std::vector<std::string> _first = { "id" };

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(id) && variable2())
		{
			*m_derivationFile << "variable -> variable2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::variable2()
{
	std::vector<std::string> _follow = { ")", ":=", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and" };

	if (m_lookAheadToken->type == OPENCUBR || m_lookAheadToken->type == DOT)
	{
		if (repIdNest1() && reptVariable())
		{
			*m_derivationFile << "variable -> repIdNest1 reptVariable\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR) && varIdNest())
		{
			*m_derivationFile << "variable -> ( aParams ) varIdNest\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "variable -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::reptVariable()
{
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and" };

	if (m_lookAheadToken->type == DOT)
	{
		if (varIdNest() && reptVariable())
		{
			*m_derivationFile << "reptVariable -> varIdNest reptVariable\n";
			return true;
		}
		else false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "reptVariable -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::varIdNest()
{
	if (m_lookAheadToken->type == DOT)
	{
		if (match(DOT) && match(id) && varIdNest2())
		{
			*m_derivationFile << "reptVariable -> . id  varIdNest2\n";
			return true;
		}
	}
	else return false;
}

bool Parser::varIdNest2()
{
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and" };

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (repIdNest1())
		{
			*m_derivationFile << "varIdNest2 -> repIdNest1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR) && varIdNest())
		{
			*m_derivationFile << "varIdNest2 -> ( aParams ) repIdNest1\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "varIdNest2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::expr()
{
	std::vector<std::string> _first = { "(", "floatLit", "id", "intLit", "not", "+", "-", "id", "."};
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->lexem == "not" || m_lookAheadToken->lexem == "+"
		|| m_lookAheadToken->lexem == "-")
	{
		if (arithExpr() && expr2())
		{
			*m_derivationFile << "expr -> arithExpr exp2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::expr2()
{
	std::vector<std::string> _first = {"<","<=","<>","==",">",">=", "EPSILON"};
	std::vector<std::string> _follow = {")",",",";"};

	if (m_lookAheadToken->lexem == "<" || m_lookAheadToken->lexem == "<=" || m_lookAheadToken->lexem == "<>" ||
		m_lookAheadToken->lexem == "==" || m_lookAheadToken->lexem == ">" || m_lookAheadToken->lexem == "<=")
	{
		if (relOp() && arithExpr())
		{
			*m_derivationFile << "expr2 -> relOp arithExpr\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == ";" || m_lookAheadToken->lexem == ")" || m_lookAheadToken->lexem == ",")
	{
		*m_derivationFile << "expr2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::arithExpr()
{
	std::vector<std::string> _first = { "(", "floatLit", "id", "intLit", "not", "+", "-" };
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->lexem == "not" || m_lookAheadToken->lexem == "+"
		|| m_lookAheadToken->lexem == "-")
	{
		if (term() && arithExpr2())
		{
			*m_derivationFile << "arithExpr -> term arithExpr2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::arithExpr2()
{
	std::vector<std::string> _first = { "+", "-", "or", "EPSILON"};
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]"};

	if(m_lookAheadToken->lexem == "+" || m_lookAheadToken->lexem == "-" || m_lookAheadToken->lexem == "or")
	{ 
		if (addOp() && term() && arithExpr2())
		{
			*m_derivationFile << "arithExpr2 -> addOp term arithExpr2\n";
			return true;
		}
		else return false;
	
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "arithExpr2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::term()
{
	std::vector<std::string> _first = { "(", "floatLit", "id", "intLit", "not", "+", "-" };
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->lexem == "not" || m_lookAheadToken->lexem == "+"
		|| m_lookAheadToken->lexem == "-")
	{
		if (factor() && rightRecTerm())
		{
			*m_derivationFile << "term -> factor rightRecTerm\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::factor()
{
	std::vector<std::string> _first = { "(", "floatLit", "id", "intLit", "not", "+", "-", "id", "."};
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "+" || m_lookAheadToken->lexem == "-")
	{
		if (sign() && factor())
		{
			*m_derivationFile << "factor -> sign factor\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "not")
	{
		if (match(TokenType::NOT) && factor())
		{
			*m_derivationFile << "factor -> 'not' factor\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::intnum)
	{
		if (match(TokenType::intnum))
		{
			*m_derivationFile << "factor -> 'intLit'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::id)
	{
		if (variable())
		{
			*m_derivationFile << "factor -> 'id' variable\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::floatnum)
	{
		if (match(TokenType::floatnum))
		{
			*m_derivationFile << "factor -> 'floatnum'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "(")
	{
		if (match(TokenType::OPENPAR) && arithExpr() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "factor -> '(' arithExpr ')'\n";
			return true;
		}
		else return false;
	}

	return true;
}

bool Parser::idOrFunction()
{
	std::vector<std::string> _first = { "[", "(", "EPSILON"};
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and", "."};

	if (m_lookAheadToken->lexem == "(")
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR))
		{
			*m_derivationFile << "idOrFunction -> '(' aParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->lexem == "[")
	{
		if (repIdNest1())
		{
			*m_derivationFile << "idOrFunction -> repIdNest1\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "idOrFunction -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::repIdNest1()
{
	std::vector<std::string> _first = { "[", "EPSILON" };
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and", ":=", "."};

	if (m_lookAheadToken->lexem == "[")
	{
		if (indice() && repIdNest1())
		{
			*m_derivationFile << "repIdNest1 -> repIdNest1\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "repIdNest1 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::rightRecTerm()
{
	std::vector<std::string> _first = { "*", "/", "and", "EPSILON" };
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=", "<>", "==", ">", ">=", "]", "+", "-", "or" };

	if (m_lookAheadToken->lexem == "*" || m_lookAheadToken->lexem == "/" || m_lookAheadToken->lexem == "and")
	{
		if (multOp() && factor() && rightRecTerm())
		{
			*m_derivationFile << "rightRecTerm -> multiOp factor rightRecTerm\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "rightRecTerm -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::reptVariableOrFunctionCall()
{
	std::vector<std::string> _first = { ".", "ESPILON"};
	std::vector<std::string> _follow = { ")", ";", ",", "<", "<=" , "<>",  "==" , ">",  ">=" , "]", "+", "-", "or", "*", "/", "and", ":="};

	if (m_lookAheadToken->lexem == ".")
	{
		if (idnest() && reptVariableOrFunctionCall())
		{
			*m_derivationFile << "reptVariableOrFunctionCall -> idnest reptVariableOrFunctionCall\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "reptVariableOrFunctionCall -> EPSILON\n";
		return true;
	}
	else return false;

}

bool Parser::relExpr()
{
	std::vector<std::string> _first = { "(", "floatLit", "id", "intLit", "not", "+", "-" };
	std::vector<std::string> _follow = {};

	if (m_lookAheadToken->lexem == "(" || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->lexem == "not" || m_lookAheadToken->lexem == "+"
		|| m_lookAheadToken->lexem == "-")
	{
		if (arithExpr() && relOp() && arithExpr())
		{
			*m_derivationFile << "relExpr -> arithExpr relOp arithExpr\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::statBlock()
{
	std::vector<std::string> _first = { "{" };

	if (m_lookAheadToken->type == OPENCUBR)
	{
		if (match(OPENCUBR) && repStatBlock() && match(CLOSECUBR))
		{
			*m_derivationFile << "statement -> '{' repStatBlock '}'\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::repStatBlock()
{
	std::vector<std::string> _first = { "if", "read", "while", "return", "write", "id", "EPSILON" };

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ || m_lookAheadToken->type == WHILE || m_lookAheadToken->type == RETURN || m_lookAheadToken->type == WRITE || m_lookAheadToken->type == id)
	{
		if (statement() && repStatBlock())
		{
			*m_derivationFile << "repStatBlock -> statement\n";
			return true;
		}
		else return false;
	}
	else if ( m_lookAheadToken->type == CLOSECUBR || m_lookAheadToken->type == ELSE || m_lookAheadToken->type == SEMI)
	{
		*m_derivationFile << "repStatBlock -> ESPILON\n";
		return true;
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
		if (match(TokenType::id) && match(TokenType::COLON) && type() && arraySize() && match(TokenType::SEMI))
		{
			*m_derivationFile << "varDec1 -> 'id' ':' type arraySize ';'\n";
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

bool Parser::idnest()
{
	std::vector<std::string> _first = { "id"};
	std::vector<std::string> _follow = { "[" };

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(DOT) && match(id) && repIdNest1())
		{
			*m_derivationFile << "idnest -> . id repIdNest1 '.'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::idnest2()
{
	std::vector<std::string> _first = { "(", "EPSILON" };
	std::vector<std::string> _follow = { "["};

	if (m_lookAheadToken->lexem == "(")
	{
		if (match(TokenType::OPENPAR) && aParams() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "idnest2 -> '(' aParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (lexemInFollowSet(_follow))
	{
		*m_derivationFile << "idnest2 -> EPSILON\n";
		return true;
	}
	else return false;
	
}


bool Parser::functionCall()
{
	return false;
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
			*m_derivationFile << "sign -> '+'\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::MINUS))
		{
			*m_derivationFile << "sign -> '-'\n";

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
			*m_derivationFile << "sign -> 'and'\n";

			return true;
		}
		else return false;

	}
	else if (_first[1] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::DIV))
		{
			*m_derivationFile << "sign -> '/'\n";

			return true;
		}
		else return false;
	}
	else if (_first[2] == m_lookAheadToken->lexem)
	{
		if (match(TokenType::MULTI))
		{
			*m_derivationFile << "sign -> '*'\n";
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

bool Parser::lexemInFollowSet(std::vector<std::string> _follow)
{
	if (std::find(_follow.begin(), _follow.end(), m_lookAheadToken->lexem) != _follow.end())
	{
		return true;
	}
	return false;
}

bool Parser::tokenInFollowSet(std::vector<TokenType> _follow)
{
	if (std::find(_follow.begin(), _follow.end(), m_lookAheadToken->type) != _follow.end())
	{
		return true;
	}
	return false;
}

bool Parser::tokenInFirstSet(std::vector<TokenType> _first)
{
	if (std::find(_first.begin(), _first.end(), m_lookAheadToken->type) != _first.end())
	{
		return true;
	}
	return false;
}




