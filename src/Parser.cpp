#include "Parser.h"

Parser::Parser(const std::string fileName, Lexer& lexer) : m_sourceFileName(fileName), m_lexer(lexer)
{
	std::cout << "Create Parser..." << std::endl;

	std::ofstream outDev(m_sourceFileName + ".outderivation", std::ofstream::out);
	std::ofstream outErrors(m_sourceFileName + ".outsyntaxerrors", std::ofstream::out);
	std::ofstream outAST(m_sourceFileName + ".outAST", std::ofstream::out);

	m_derivationFile = &outDev;
	m_syntaxErrorsFile = &outErrors;
	m_ASTFile = &outAST;

	// Assignment 3
	m_nodeFactory = new NodeFactory();

	if (this->parse())
	{
		std::cout << "-------------- Successful Parse" << std::endl;
	}
	else
	{
		std::cout << "-------------- Failed to Parse" << std::endl; 
	}

	writeAST(m_astRoot, 0);

	// Close Files 
	m_derivationFile->close();
	m_syntaxErrorsFile->close();
	m_ASTFile->close();

	m_derivationFile = nullptr;
	m_syntaxErrorsFile = nullptr;
	m_ASTFile = nullptr;
	
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
	if (startsymbol() && match(END_OF_FILE)) return true;
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

bool Parser::skipErrors(bool containsESPILON, std::vector<TokenType> first, std::vector<TokenType> follow)
{

	if(std::find(first.begin(), first.end(), m_lookAheadToken->type) != first.end() || 
		(containsESPILON &&	std::find(follow.begin(), follow.end(), m_lookAheadToken->type) != follow.end()))
	{
		return true;
	}
	else
	{
		*m_syntaxErrorsFile << "Syntax error at line:  " + std::to_string(m_lookAheadToken->position) << std::endl;

		while (!(std::find(first.begin(), first.end(), m_lookAheadToken->type) != first.end() ||
			std::find(follow.begin(), follow.end(), m_lookAheadToken->type) != follow.end()))
		{
			if (!nextToken()) return false;

			if (containsESPILON && std::find(follow.begin(), follow.end(), m_lookAheadToken->type) != follow.end())
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
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR, CLASS, IMPLEMENTATION };
	std::vector<TokenType> follow = { END_OF_FILE };

	if (!skipErrors(true, first, follow)) return false;

	// Check First 
	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR ||
		m_lookAheadToken->type == CLASS || m_lookAheadToken->type == IMPLEMENTATION)
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
		*m_derivationFile << "startsymbol -> EPSILON\n";
		return true;
	}
	else return false;

}

bool Parser::prog()
{
	m_astRoot = m_nodeFactory->makeNode(Type::prog);

	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR, CLASS, IMPLEMENTATION };
	std::vector<TokenType> follow = { END_OF_FILE };

	if (!skipErrors(true, first, follow)) return false;

	// Check First 
	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR ||
		m_lookAheadToken->type == CLASS || m_lookAheadToken->type == IMPLEMENTATION)
	{
		if (classOrImplOrFunc(m_astRoot) && prog())
		{
			*m_derivationFile << "prog -> classOrImplOrFunc prog\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::END_OF_FILE)
	{
		*m_derivationFile << "prog -> EPSILON\n";
		return true;
	}
	else return false;

}

bool Parser::classOrImplOrFunc(Node* root)
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR, CLASS, IMPLEMENTATION };
	std::vector<TokenType> follow = { };

	Node* classDeclList_Node = m_nodeFactory->makeNode(Type::classDeclList);
	root->children.push_back(classDeclList_Node);

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcDef())
		{
			*m_derivationFile << "classOrImplOrFunc -> funcDef\n";
			root = classDeclList_Node;
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == CLASS)
	{
		if (classDecl())
		{
			*m_derivationFile << "classOrImplOrFunc -> classDecl\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IMPLEMENTATION)
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

bool Parser::classDecl()
{
	std::vector<TokenType> first = { CLASS };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CLASS)
	{
		if (match(TokenType::CLASS) && match(TokenType::id) && ISA1() && match(TokenType::OPENCUBR) &&
			VISMEMBERDECL() && match(TokenType::CLOSECUBR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "classDecl -> 'class' 'id' ISA1 '{' VISMEMBERDECL '}' ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::VISMEMBERDECL()
{
	std::vector<TokenType> first = { PRIVATE, PUBLIC };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == PRIVATE || m_lookAheadToken->type == PUBLIC)
	{
		if (visibility() && memberDecl() && VISMEMBERDECL())
		{
			*m_derivationFile << "VISMEMBERDECL -> visibility memberDec1 VISMEMBERDECL\n";
			return true;
		}
		else return false;
	}
	
	else if (m_lookAheadToken->type == CLOSECUBR)
	{
		*m_derivationFile << "VISMEMBERDECL -> EPSILON\n";
		return true;
	}
	else return false;

}

bool Parser::ISA1()
{
	std::vector<TokenType> first = { ISA };
	std::vector<TokenType> follow = { OPENCUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == ISA)
	{
		if (match(TokenType::ISA) && match(TokenType::id) && ISA2())
		{
			*m_derivationFile << "ISA1 -> 'isa' 'id' ISA2\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "ISA1 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::ISA2()
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { OPENCUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == ISA)
	{
		if (match(TokenType::COMMA) && match(TokenType::id) && ISA2())
		{
			*m_derivationFile << "ISA2 -> ',' 'id' ISA2\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "ISA2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::implDef()
{
	std::vector<TokenType> first = { IMPLEMENTATION };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == IMPLEMENTATION)
	{
		if (match(TokenType::IMPLEMENTATION) && match(TokenType::id) && match(TokenType::OPENCUBR) && impleBody() && match(TokenType::CLOSECUBR))
		{
			*m_derivationFile << "implDef -> 'implementation' 'id' '{' funcDef '}'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::impleBody()
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcDef() && impleBody())
		{
			*m_derivationFile << "impleBody -> funcDef impleBody\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "impleBody -> ESPILON\n";
		return true;
	}
	else return false;
}

bool Parser::funcDef()
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcHead() && funcBody())
		{
			*m_derivationFile << "funcDef -> funcHead funcBody\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::visibility()
{
	std::vector<TokenType> first = { PRIVATE, PUBLIC };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == PRIVATE)
	{
		if (match(TokenType::PRIVATE))
		{
			*m_derivationFile << "visibility -> 'private'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == PUBLIC)
	{
		if (match(TokenType::PUBLIC))
		{
			*m_derivationFile << "visibility -> 'public'\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::memberDecl()
{
	std::vector<TokenType> first = { CONSTRUCTOR, ATTRIBUTE, FUNCTION };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CONSTRUCTOR || m_lookAheadToken->type == FUNCTION)
	{
		if (funcDec1())
		{
			*m_derivationFile << "memberDecl -> funcDec1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == ATTRIBUTE)
	{
		if (attributeDecl())
		{
			*m_derivationFile << "memberDecl -> attributeDec1\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::funcDec1()
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CONSTRUCTOR || m_lookAheadToken->type == FUNCTION)
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
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (match(TokenType::CONSTRUCTOR) && match(TokenType::OPENPAR) && fParams() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "funcHead -> 'constructor' '(' fParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == FUNCTION)
	{
		if (match(TokenType::FUNCTION) && match(TokenType::id) && match(TokenType::OPENPAR) && fParams() && match(TokenType::CLOSEPAR) && match(TokenType::ARROW) && returnType())
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
	std::vector<TokenType> first = { OPENCUBR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENCUBR)
	{
		if (match(TokenType::OPENCUBR) && LOCALVARDECLORSTAT2() && match(TokenType::CLOSECUBR))
		{
			*m_derivationFile << "funcBody -> '{' localVarDeclOrStatRep '}'\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::LOCALVARDECLORSTAT()
{
	std::vector<TokenType> first = { LOCAL, IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ
		|| m_lookAheadToken->type == WHILE || m_lookAheadToken->type == WRITE
		|| m_lookAheadToken->type == RETURN || m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (statement())
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> statement\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LOCAL)
	{
		if (localVarDecl())
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> localVarDecl\n";
			return true;
		}
		else return false;
	}

	else return false;
}

bool Parser::LOCALVARDECLORSTAT2()
{
	std::vector<TokenType> first = { LOCAL, IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ
		|| m_lookAheadToken->type == WHILE || m_lookAheadToken->type == WRITE
		|| m_lookAheadToken->type == RETURN || m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF || m_lookAheadToken->type == TokenType::LOCAL)
	{
		if (LOCALVARDECLORSTAT() && LOCALVARDECLORSTAT2())
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> LOCALVARDECLORSTAT LOCALVARDECLORSTAT2\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LOCAL)
	{
		if (localVarDecl())
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> localVarDecl\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "LOCALVARDECLORSTAT2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::attributeDecl()
{
	std::vector<TokenType> first = { ATTRIBUTE };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == ATTRIBUTE)
	{
		if (match(TokenType::ATTRIBUTE) && varDecl())
		{
			*m_derivationFile << "attributeDecl -> 'attribute' varDecl\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::localVarDecl()
{
	std::vector<TokenType> first = { LOCAL };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == LOCAL)
	{
		if (match(TokenType::LOCAL) && varDecl())
		{
			*m_derivationFile << "localVarDecl -> 'local' varDecl ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::varDecl()
{
	std::vector<TokenType>  first = { id };
	std::vector<TokenType>  follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(TokenType::id) && match(TokenType::COLON) && type() && arraySizes() && match(TokenType::SEMI))
		{
			*m_derivationFile << "varDec1 -> 'id' ':' type arraySize ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::statement()
{
	std::vector<TokenType> first = { id, SELF, WRITE, WHILE, RETURN, READ, IF };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (FUNCALLORASSIGN() && match(SEMI))
		{
			*m_derivationFile << "statement -> FUNCALLORASSIGN ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == WRITE)
	{
		if (match(TokenType::WRITE) && match(TokenType::OPENPAR) && expr() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'write' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == WHILE)
	{
		if (match(TokenType::WHILE) && match(TokenType::OPENPAR) && relExpr() && match(TokenType::CLOSEPAR) && statBlock() && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'while' '(' relExpr ')' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == RETURN)
	{
		if (match(TokenType::RETURN) && match(TokenType::OPENPAR) && expr() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'return' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == READ)
	{
		if (match(TokenType::READ) && match(TokenType::OPENPAR) && variable() && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'read' '(' reptVariableOrFunctionCall ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IF)
	{
		if (match(TokenType::IF) && match(TokenType::OPENPAR) && relExpr() && match(CLOSEPAR) && match(TokenType::THEN) && statBlock() &&
			match(TokenType::ELSE) && statBlock() && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'if' '(' relExpr ')' 'then' statBlock 'else' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN()
{
	std::vector<TokenType> first = { id, SELF};
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		if (IDORSELF() && FUNCALLORASSIGN2())
		{
			*m_derivationFile << "FUNCALLORASSIGN -> IDORSELF FUNCALLORASSIGN2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN2()
{
	std::vector<TokenType> first = { OPENSQBR, DOT, ASSIGN, OPENPAR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR || m_lookAheadToken->type == DOT || m_lookAheadToken->type == ASSIGN)
	{
		if (indices() && FUNCALLORASSIGN3())
		{
			*m_derivationFile << "FUNCALLORASSIGN2 -> indice FUNCALLORASSIGN3\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR) && FUNCALLORASSIGN4())
		{
			*m_derivationFile << "FUNCALLORASSIGN2 -> '(' aParams ')' FUNCALLORASSIGN4\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN3()
{
	std::vector<TokenType> first = { DOT, ASSIGN};
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == ASSIGN)
	{
		if (assignOp() && expr())
		{
			*m_derivationFile << "FUNCALLORASSIGN3 -> assignOp expr\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == DOT)
	{
		if (match(DOT) && match(id) && FUNCALLORASSIGN2())
		{
			*m_derivationFile << "FUNCALLORASSIGN3 -> '.' 'id' FUNCALLORASSIGN2\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::FUNCALLORASSIGN4()
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { SEMI };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (match(id) && match(id) && FUNCALLORASSIGN2())
		{
			*m_derivationFile << "FUNCALLORASSIGN4 -> '.' 'id' FUNCALLORASSIGN2\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "FUNCALLORASSIGN4 -> 'EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::statBlock()
{
	std::vector<TokenType> first = { IF, READ, RETURN, WHILE, WRITE, id, SELF, OPENCUBR };
	std::vector<TokenType> follow = {ELSE, SEMI};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENCUBR)
	{
		if (match(OPENCUBR) && STATEMENTS() && match(CLOSECUBR))
		{
			*m_derivationFile << "statBlock -> '{' STATEMENTS '}'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ || m_lookAheadToken->type == WHILE || m_lookAheadToken->type == RETURN || m_lookAheadToken->type == WRITE || m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		if (statement())
		{
			*m_derivationFile << "statBlock -> statement\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "statBlock -> EPSILON\n";
		return true;
	}
	else return false;

}

bool Parser::STATEMENTS()
{
	std::vector<TokenType> first = { IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ || m_lookAheadToken->type == WHILE || m_lookAheadToken->type == RETURN || m_lookAheadToken->type == WRITE || m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		if (statement() && STATEMENTS())
		{
			*m_derivationFile << "STATEMENTS -> statement STATEMENTS\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "STATEMENTS -> ESPILON\n";
		return true;
	}
	else return false;
}

bool Parser::expr()
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
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
	std::vector<TokenType> first = { EQ, GT, GEQ, LT, LEQ, NOTEQ };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == EQ || m_lookAheadToken->type == GT || m_lookAheadToken->type == GEQ ||
		m_lookAheadToken->type == LT || m_lookAheadToken->type == LEQ || m_lookAheadToken->type == NOTEQ)
	{
		if (relOp() && arithExpr())
		{
			*m_derivationFile << "expr2 -> relOp arithExpr\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "expr2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::relExpr()
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
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

bool Parser::arithExpr()
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
	{
		if (term() && RIGHTRECARITHEXPR())
		{
			*m_derivationFile << "arithExpr -> term RIGHTRECARITHEXPR\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::RIGHTRECARITHEXPR()
{
	std::vector<TokenType> first = { MINUS, OR, PLUS, };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT , GEQ,  LT , LEQ,  NOTEQ , CLOSESQBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == PLUS || m_lookAheadToken->type == MINUS || m_lookAheadToken->type == OR)
	{
		if (addOp() && term() && RIGHTRECARITHEXPR())
		{
			*m_derivationFile << "RIGHTRECARITHEXPR -> addOp term arithExpr2\n";
			return true;
		}
		else return false;

	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "RIGHTRECARITHEXPR -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::sign()
{
	std::vector<TokenType> first = { PLUS, MINUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == PLUS)
	{
		if (match(PLUS))
		{
			*m_derivationFile << "sign -> '+'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == MINUS)
	{
		if (match(MINUS))
		{
			*m_derivationFile << "sign -> '-'\n";

			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::term()
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR 
		|| m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id 
		|| m_lookAheadToken->type == TokenType::SELF 
		|| m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT 
		|| m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS)
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

bool Parser::rightRecTerm()
{
	std::vector<TokenType> first = { MULTI, DIV, AND };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == MULTI || m_lookAheadToken->type == DIV || m_lookAheadToken->type == AND)
	{
		if (multOp() && factor() && rightRecTerm())
		{
			*m_derivationFile << "rightRecTerm -> multiOp factor rightRecTerm\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "rightRecTerm -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::factor()
{
	std::vector<TokenType> first = { MINUS, PLUS, id, SELF, NOT, intnum, floatnum, OPENPAR };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == MINUS || m_lookAheadToken->type == PLUS)
	{
		if (sign() && factor())
		{
			*m_derivationFile << "factor -> sign factor\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (IDORSELF() && factor2() && REPTVARIABLEORFUNCTIONCALL())
		{
			*m_derivationFile << "factor -> IDORSELF factor2 REPTVARIABLEORFUNCTIONCALL\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == NOT)
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
	else if (m_lookAheadToken->type == TokenType::floatnum)
	{
		if (match(TokenType::floatnum))
		{
			*m_derivationFile << "factor -> 'floatnum'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(TokenType::OPENPAR) && arithExpr() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "factor -> '(' arithExpr ')'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::factor2()
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND, DOT };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indices())
		{
			*m_derivationFile << "factor2 -> indice\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR))
		{
			*m_derivationFile << "factor2 -> '(' aParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "factor2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::indice()
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (match(OPENSQBR) && arithExpr() && match(CLOSESQBR))
		{
			*m_derivationFile << "indice -> '(' arithExpr ')'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::indices()
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { DOT, ASSIGN, CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indice() && indices())
		{
			*m_derivationFile << "indices -> indice indices\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "indices -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::REPTVARIABLEORFUNCTIONCALL()
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (idnest() && REPTVARIABLEORFUNCTIONCALL())
		{
			*m_derivationFile << "REPTVARIABLEORFUNCTIONCALL -> idnest REPTVARIABLEORFUNCTIONCALL\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "REPTVARIABLEORFUNCTIONCALL -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::variable()
{
	std::vector<TokenType> first = { id, SELF };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (IDORSELF() && variable2())
		{
			*m_derivationFile << "variable -> IDORSELF variable2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::variable2()
{
	std::vector<TokenType> first = { OPENSQBR, DOT, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR || m_lookAheadToken->type == DOT)
	{
		if (indices() && reptVariable())
		{
			*m_derivationFile << "variable2 -> indices reptVariable\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR) && varIdNest())
		{
			*m_derivationFile << "variable -> '(' aParams ')' varIdNest\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "variable -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::reptVariable()
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (varIdNest() && reptVariable())
		{
			*m_derivationFile << "reptVariable -> varIdNest reptVariable\n";
			return true;
		}
		else false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "reptVariable -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::varIdNest()
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (match(DOT) && match(id) && varIdNest2())
		{
			*m_derivationFile << "reptVariable -> '.' 'id'  varIdNest2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::varIdNest2()
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, DOT};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indices())
		{
			*m_derivationFile << "varIdNest2 -> indices\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams() && match(CLOSEPAR) && varIdNest())
		{
			*m_derivationFile << "varIdNest2 -> '(' aParams ')' varIdNest\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "varIdNest2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::idnest()
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::DOT)
	{
		if (match(DOT) && match(id) && idnest2())
		{
			*m_derivationFile << "idnest -> '.' 'id' idnest2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::idnest2()
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND, DOT};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(TokenType::OPENPAR) && aParams() && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "idnest2 -> '(' aParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indices())
		{
			*m_derivationFile << "idnest2 -> indices\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "idnest2 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::arraySize()
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (match(TokenType::OPENSQBR) && arraySize2())
		{
			*m_derivationFile << "arraySize -> '[' arraySize2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::arraySize2()
{
	std::vector<TokenType> first = { CLOSESQBR, intnum };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CLOSESQBR)
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

bool Parser::arraySizes()
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { SEMI, CLOSEPAR, COMMA };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (arraySize() && arraySizes())
		{
			*m_derivationFile << "arraySizes -> arraySize arraySizes\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "arraySizes -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::type()
{
	std::vector<TokenType> first = { INT, FLOAT, id };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == INT)
	{
		if (match(TokenType::INT))
		{
			*m_derivationFile << "type -> 'int'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == FLOAT)
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


bool Parser::returnType()
{
	std::vector<TokenType> first = { FLOAT, INT, id, VOID };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

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
	else if (m_lookAheadToken->type == VOID)
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

bool Parser::aParams()
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR
		|| m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id 
		|| m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT
		|| m_lookAheadToken->type == PLUS 
		|| m_lookAheadToken->type == MINUS)
	{
		if (expr() && REPTAPARAMS1())
		{
			*m_derivationFile << "aParams -> expr REPTAPARAMS1\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "aParams -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::REPTAPARAMS1()
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (aParamsTail() && REPTAPARAMS1())
		{
			*m_derivationFile << "REPTAPARAMS1 -> aParamsTail REPTAPARAMS1\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "REPTAPARAMS1 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::aParamsTail()
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (match(TokenType::COMMA) && expr())
		{
			*m_derivationFile << "aParamsTail -> ',' expr\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::fParams()
{
	std::vector<TokenType> first = { id };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id)
	{
		if (match(TokenType::id) && match(TokenType::COLON) && type() && arraySizes() && REPTFPARAMS1())
		{
			*m_derivationFile << "fParams -> 'id' ':' type arraySizes REPTFPARAMS1\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "fParams -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::REPTFPARAMS1()
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (fParamsTail() && REPTFPARAMS1())
		{
			*m_derivationFile << "REPTFPARAMS1 -> fParamsTail REPTFPARAMS1\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		*m_derivationFile << "REPTFPARAMS1 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::fParamsTail()
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (match(TokenType::COMMA) && match(TokenType::id) && match(TokenType::COLON) && type() && arraySizes())
		{
			*m_derivationFile << "fParasTail -> ',' 'id' ':' type arraySizes\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::assignOp()
{
	std::vector<TokenType> first = { ASSIGN };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == ASSIGN)
	{
		if (match(ASSIGN))
		{
			*m_derivationFile << "assignOp -> :=";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::relOp()
{
	std::vector<TokenType> first = { NOTEQ, LEQ, LT, GEQ, GT, EQ };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == GEQ)
	{
		if (match(TokenType::GEQ))
		{
			*m_derivationFile << "relOp -> '>='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == GT)
	{
		if (match(TokenType::GT))
		{
			*m_derivationFile << "relOp -> '>'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == EQ)
	{
		if (match(TokenType::EQ))
		{
			*m_derivationFile << "relOp -> '=='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LEQ)
	{
		if (match(TokenType::LEQ))
		{
			*m_derivationFile << "relOp -> '<='\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LT)
	{
		if (match(TokenType::LT))
		{
			*m_derivationFile << "relOp -> '<'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == NOTEQ)
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

bool Parser::addOp()
{
	std::vector<TokenType> first = { OR, PLUS, MINUS };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OR)
	{
		if (match(TokenType::OR))
		{
			*m_derivationFile << "addOp -> 'or'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == PLUS)
	{
		if (match(TokenType::PLUS))
		{
			*m_derivationFile << "addOp -> '+'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == MINUS)
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

bool Parser::multOp()
{
	std::vector<TokenType> first = { AND, DIV, MULTI };
	std::vector<TokenType>  follow = {};

	if (!skipErrors(false, first, follow)) return false;

	// Check First 
	if (m_lookAheadToken->type == AND)
	{
		if (match(TokenType::AND))
		{
			*m_derivationFile << "sign -> 'and'\n";

			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type = DIV)
	{
		if (match(TokenType::DIV))
		{
			*m_derivationFile << "sign -> '/'\n";

			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == MULTI)
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

bool Parser::IDORSELF()
{
	std::vector<TokenType> first = { id, SELF };
	std::vector<TokenType>  follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == id)
	{
		if (match(id))
		{
			*m_derivationFile << "IDORSELF -> 'id'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == SELF)
	{
		if (match(SELF))
		{
			*m_derivationFile << "IDORSELF -> 'SELF'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////
//////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////

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

void Parser::writeAST(Node* root, int level)
{
	if (root == nullptr) {
		return;
	}

	// Print the current node's data
	// Indent according to the level of the node
	*m_ASTFile << std::string(level, ' ') << "| " << root->getType() << std::endl;

	// Print each child node, with a '|' prefix
	for (Node* child : root->children) {
		// Recursively print each child's children
		writeAST(child, level + 2);
	}

}

