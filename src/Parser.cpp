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
	if (m_astRoot == nullptr)
	{
		m_astRoot = m_nodeFactory->makeNode(Type::prog);	
	}

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
	if (!root->hasChild(Type::classDeclList))
	{
		root->addChild(classDeclList_Node);
	}
	else
	{
		classDeclList_Node = root->getChild(Type::classDeclList);
	}

	Node* implDefList_Node = m_nodeFactory->makeNode(Type::implDefList);
	if (!root->hasChild(Type::implDefList))
	{
		root->addChild(implDefList_Node);
	}
	else
	{
		implDefList_Node = root->getChild(Type::implDefList);
	}

	Node* funcDefList_Node = m_nodeFactory->makeNode(Type::funcDefList);
	if (!root->hasChild(Type::funcDefList))
	{
		root->addChild(funcDefList_Node);
	}
	else
	{
		funcDefList_Node = root->getChild(Type::funcDefList);
	}
	
	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcDef(funcDefList_Node))
		{
			*m_derivationFile << "classOrImplOrFunc -> funcDef\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == CLASS)
	{
		if (classDecl(classDeclList_Node))
		{
			*m_derivationFile << "classOrImplOrFunc -> classDecl\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IMPLEMENTATION)
	{
		if (implDef(implDefList_Node))
		{
			*m_derivationFile << "classOrImplOrFunc -> implDef\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::classDecl(Node* root)
{
	std::vector<TokenType> first = { CLASS };
	std::vector<TokenType> follow = { };

	Node* classDecl_Node = m_nodeFactory->makeNode(Type::classDecl);
	Node* memDeclList_Node = m_nodeFactory->makeNode(Type::memDeclList);
	root->addChild(classDecl_Node);
	classDecl_Node->addChild(m_nodeFactory->makeNode(Type::idLit));

	classDecl_Node->token = m_lexer.peekAheadToken();

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CLASS)
	{
		if (match(TokenType::CLASS) && match(TokenType::id) && ISA1(classDecl_Node) && match(TokenType::OPENCUBR) &&
			VISMEMBERDECL(memDeclList_Node) && match(TokenType::CLOSECUBR) && match(TokenType::SEMI))
		{
			classDecl_Node->addChild(memDeclList_Node);
			*m_derivationFile << "classDecl -> 'class' 'id' ISA1 '{' VISMEMBERDECL '}' ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::VISMEMBERDECL(Node* root)
{
	std::vector<TokenType> first = { PRIVATE, PUBLIC };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == PRIVATE || m_lookAheadToken->type == PUBLIC)
	{
		if (visibility() && memberDecl(root) && VISMEMBERDECL(root))
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

bool Parser::ISA1(Node* root)
{
	std::vector<TokenType> first = { ISA };
	std::vector<TokenType> follow = { OPENCUBR };

	Node* inheritList_Node = m_nodeFactory->makeNode(Type::inheritList);

	Node* id_node = m_nodeFactory->makeNode(Type::idLit);
	id_node->token = m_lexer.peekAheadToken();

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == ISA)
	{
		if (match(TokenType::ISA) && match(TokenType::id) && ISA2(inheritList_Node))
		{
			root->addChild(inheritList_Node);
			inheritList_Node->addChild(id_node);
			*m_derivationFile << "ISA1 -> 'isa' 'id' ISA2\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		root->addChild(inheritList_Node);
		*m_derivationFile << "ISA1 -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::ISA2(Node* root)
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { OPENCUBR };

	if (!skipErrors(true, first, follow)) return false;

	Node* id_node = m_nodeFactory->makeNode(Type::idLit);
	id_node->token = m_lexer.peekAheadToken();

	if (m_lookAheadToken->type == COMMA)
	{
		if (match(TokenType::COMMA) && match(TokenType::id) && ISA2(root))
		{
			root->addChild(id_node);
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

bool Parser::implDef(Node* root)
{
	std::vector<TokenType> first = { IMPLEMENTATION };
	std::vector<TokenType> follow = { };

	Node* impleDef_Node = m_nodeFactory->makeNode(Type::impleDef);

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == IMPLEMENTATION)
	{
		impleDef_Node->addChild(m_nodeFactory->makeNode(idLit));
		impleDef_Node->token = m_lexer.peekAheadToken();

		if (match(TokenType::IMPLEMENTATION) && match(TokenType::id) && match(TokenType::OPENCUBR) && impleBody(impleDef_Node) && match(TokenType::CLOSECUBR))
		{
			root->addChild(impleDef_Node);
			*m_derivationFile << "implDef -> 'implementation' 'id' '{' funcDef '}'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::impleBody(Node* root)
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcDef(root) && impleBody(root))
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

bool Parser::funcDef(Node* root)
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	Node* funcDef = m_nodeFactory->makeNode(Type::funcDef);
	root->addChild(funcDef);

	funcDef->token = m_lexer.peekAheadToken();

	if (m_lookAheadToken->type == FUNCTION || m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (funcHead(funcDef) && funcBody(funcDef))
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

bool Parser::memberDecl(Node* root)
{
	std::vector<TokenType> first = { CONSTRUCTOR, ATTRIBUTE, FUNCTION };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CONSTRUCTOR || m_lookAheadToken->type == FUNCTION)
	{
		Node* memDeclFunc_Node = m_nodeFactory->makeNode(Type::memDeclFunc);
		memDeclFunc_Node->token = m_lexer.peekAheadToken();
		root->addChild(memDeclFunc_Node);

		if (funcDec1(memDeclFunc_Node))
		{
			*m_derivationFile << "memberDecl -> funcDec1\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == ATTRIBUTE)
	{
		Node* memDeclAttrib_Node = m_nodeFactory->makeNode(Type::memDeclAttrib);
		root->addChild(memDeclAttrib_Node);

		if (attributeDecl(memDeclAttrib_Node))
		{
			*m_derivationFile << "memberDecl -> attributeDec1\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::funcDec1(Node* root)
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == CONSTRUCTOR || m_lookAheadToken->type == FUNCTION)
	{
		
		if (funcHead(root) && match(TokenType::SEMI))
		{
			*m_derivationFile << "funcDec1 -> funcHead\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::funcHead(Node* root)
{
	std::vector<TokenType> first = { FUNCTION, CONSTRUCTOR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* fParams_node = m_nodeFactory->makeNode(Type::fParamsList);
	fParams_node->token = m_lookAheadToken;

	if (m_lookAheadToken->type == CONSTRUCTOR)
	{
		if (match(TokenType::CONSTRUCTOR) && match(TokenType::OPENPAR) && fParams(root) && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "funcHead -> 'constructor' '(' fParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == FUNCTION)
	{
		
		if (match(TokenType::FUNCTION) && match(TokenType::id) && match(TokenType::OPENPAR) && fParams(fParams_node) && match(TokenType::CLOSEPAR) && match(TokenType::ARROW) && returnType(root))
		{
			*m_derivationFile << "'function' 'id' '(' fParams ')' '=>' returnType\n";
			root->addChild(m_nodeFactory->makeNode(Type::idLit));
			root->addChild(fParams_node);
			root->addChild(m_nodeFactory->makeNode(Type::type));
			// Make subtree from everything
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::funcBody(Node* root)
{
	std::vector<TokenType> first = { OPENCUBR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;
	
	Node* statBlock_Node = m_nodeFactory->makeNode(Type::statBlock);

	if (m_lookAheadToken->type == OPENCUBR)
	{
		if (match(TokenType::OPENCUBR) && LOCALVARDECLORSTAT2(statBlock_Node) && match(TokenType::CLOSECUBR))
		{
			root->addChild(statBlock_Node);
			*m_derivationFile << "funcBody -> '{' localVarDeclOrStatRep '}'\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::LOCALVARDECLORSTAT(Node* root)
{
	std::vector<TokenType> first = { LOCAL, IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ
		|| m_lookAheadToken->type == WHILE || m_lookAheadToken->type == WRITE
		|| m_lookAheadToken->type == RETURN || m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (statement(root))
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> statement\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LOCAL)
	{
		if (localVarDecl(root))
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> localVarDecl\n";
			return true;
		}
		else return false;
	}

	else return false;
}

bool Parser::LOCALVARDECLORSTAT2(Node* root)
{
	std::vector<TokenType> first = { LOCAL, IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ
		|| m_lookAheadToken->type == WHILE || m_lookAheadToken->type == WRITE
		|| m_lookAheadToken->type == RETURN || m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF || m_lookAheadToken->type == TokenType::LOCAL)
	{
		if (LOCALVARDECLORSTAT(root) && LOCALVARDECLORSTAT2(root))
		{
			*m_derivationFile << "LOCALVARDECLORSTAT2 -> LOCALVARDECLORSTAT LOCALVARDECLORSTAT2\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == LOCAL)
	{
		if (localVarDecl(root))
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

bool Parser::attributeDecl(Node* root)
{
	std::vector<TokenType> first = { ATTRIBUTE };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == ATTRIBUTE)
	{
		if (match(TokenType::ATTRIBUTE) && varDecl(root))
		{
			*m_derivationFile << "attributeDecl -> 'attribute' varDecl\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::localVarDecl(Node* root)
{
	std::vector<TokenType> first = { LOCAL };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == LOCAL)
	{
		if (match(TokenType::LOCAL) && varDecl(root))
		{
			*m_derivationFile << "localVarDecl -> 'local' varDecl ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::varDecl(Node* root)
{
	std::vector<TokenType>  first = { id };
	std::vector<TokenType>  follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* arraySizeList_node = m_nodeFactory->makeNode(Type::arraySizeList);
	Node* varDecl_Node = m_nodeFactory->makeNode(Type::varDecl);

	if (m_lookAheadToken->type == TokenType::id)
	{
		Node* id_node = m_nodeFactory->makeNode(Type::idLit);
		id_node->token = m_lookAheadToken;
		varDecl_Node->addChild(id_node);

		if (match(TokenType::id) && match(TokenType::COLON) && type(varDecl_Node) && arraySizes(arraySizeList_node) && match(TokenType::SEMI))
		{
			
			varDecl_Node->addChild(arraySizeList_node);

			root->addChild(varDecl_Node);
			*m_derivationFile << "varDec1 -> 'id' ':' type arraySize ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::statement(Node* root)
{
	std::vector<TokenType> first = { id, SELF, WRITE, WHILE, RETURN, READ, IF };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		if (FUNCALLORASSIGN(root) && match(SEMI))
		{
			*m_derivationFile << "statement -> FUNCALLORASSIGN ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == WRITE)
	{
		Node* writeStat_Node = m_nodeFactory->makeNode(Type::writeStat);
		root->addChild(writeStat_Node);

		if (match(TokenType::WRITE) && match(TokenType::OPENPAR) && expr(writeStat_Node) && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'write' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == WHILE)
	{
		Node* whileStat_Node = m_nodeFactory->makeNode(Type::whileStat);
		root->addChild(whileStat_Node);

		if (match(TokenType::WHILE) && match(TokenType::OPENPAR) && relExpr(whileStat_Node) && match(TokenType::CLOSEPAR) && statBlock(whileStat_Node) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'while' '(' relExpr ')' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == RETURN)
	{
		Node* returnStat_Node = m_nodeFactory->makeNode(Type::returnStat);
		root->addChild(returnStat_Node);

		if (match(TokenType::RETURN) && match(TokenType::OPENPAR) && expr(returnStat_Node) && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'return' '(' expr ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == READ)
	{
		Node* readStat_Node = m_nodeFactory->makeNode(Type::readStat);
		root->addChild(readStat_Node);

		if (match(TokenType::READ) && match(TokenType::OPENPAR) && variable(readStat_Node) && match(TokenType::CLOSEPAR) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'read' '(' reptVariableOrFunctionCall ')' ';'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IF)
	{
		Node* ifStat_Node = m_nodeFactory->makeNode(Type::ifStat);
		root->addChild(ifStat_Node);

		if (match(TokenType::IF) && match(TokenType::OPENPAR) && relExpr(ifStat_Node) && match(CLOSEPAR) && match(TokenType::THEN) && statBlock(ifStat_Node) &&
			match(TokenType::ELSE) && statBlock(ifStat_Node) && match(TokenType::SEMI))
		{
			*m_derivationFile << "statement -> 'if' '(' relExpr ')' 'then' statBlock 'else' statBlock ';'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN(Node* root)
{
	std::vector<TokenType> first = { id, SELF};
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		Node* id_node = m_nodeFactory->makeNode(idLit);

		if (IDORSELF(id_node) && FUNCALLORASSIGN2(*id_node, root))
		{
			*m_derivationFile << "FUNCALLORASSIGN -> IDORSELF FUNCALLORASSIGN2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN2(Node& child, Node* root)
{
	std::vector<TokenType> first = { OPENSQBR, DOT, ASSIGN, OPENPAR };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR || m_lookAheadToken->type == DOT || m_lookAheadToken->type == ASSIGN)
	{
		if (indices(&child) && FUNCALLORASSIGN3(child, root))
		{
			*m_derivationFile << "FUNCALLORASSIGN2 -> indice FUNCALLORASSIGN3\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		root->setType(Type::fCall);
		Node* aParams_Node = m_nodeFactory->makeNode(Type::aParams);

		if (match(OPENPAR) && aParams(aParams_Node) && match(CLOSEPAR) && FUNCALLORASSIGN4(child, root))
		{
			root->addChild(aParams_Node);
			*m_derivationFile << "FUNCALLORASSIGN2 -> '(' aParams ')' FUNCALLORASSIGN4\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::FUNCALLORASSIGN3(Node& child, Node* root)
{
	std::vector<TokenType> first = { DOT, ASSIGN};
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == ASSIGN)
	{
		Node* assignOp_Node = m_nodeFactory->makeNode(Type::assignOp);
		assignOp_Node->addChild(&child);
		root->addChild(assignOp_Node);

		if (assignOp(root) && expr(assignOp_Node))
		{
			*m_derivationFile << "FUNCALLORASSIGN3 -> assignOp expr\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == DOT)
	{
		Node* dot_Node = m_nodeFactory->makeNode(Type::dot);
		dot_Node->addChild(&child);
		root->addChild(dot_Node);

		if (match(DOT) && match(id) && FUNCALLORASSIGN2(child, dot_Node))
		{
			*m_derivationFile << "FUNCALLORASSIGN3 -> '.' 'id' FUNCALLORASSIGN2\n";
			return true;
		}
		else return false;
	}
	else return false;

}

bool Parser::FUNCALLORASSIGN4(Node& child, Node* root)
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { SEMI };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (match(DOT) && match(id) && FUNCALLORASSIGN2(child, root->parent))
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

bool Parser::statBlock(Node* root)
{
	std::vector<TokenType> first = { IF, READ, RETURN, WHILE, WRITE, id, SELF, OPENCUBR };
	std::vector<TokenType> follow = {ELSE, SEMI};

	if (!skipErrors(true, first, follow)) return false;

	Node* statBlock_Node = m_nodeFactory->makeNode(Type::statBlock);
	root->addChild(statBlock_Node);

	if (m_lookAheadToken->type == OPENCUBR)
	{
		if (match(OPENCUBR) && STATEMENTS(statBlock_Node) && match(CLOSECUBR))
		{
			*m_derivationFile << "statBlock -> '{' STATEMENTS '}'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ || m_lookAheadToken->type == WHILE || m_lookAheadToken->type == RETURN || m_lookAheadToken->type == WRITE || m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		if (statement(statBlock_Node))
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

bool Parser::STATEMENTS(Node* root)
{
	std::vector<TokenType> first = { IF, READ, RETURN, WHILE, WRITE, id, SELF };
	std::vector<TokenType> follow = { CLOSECUBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == IF || m_lookAheadToken->type == READ || m_lookAheadToken->type == WHILE || m_lookAheadToken->type == RETURN || m_lookAheadToken->type == WRITE || m_lookAheadToken->type == id || m_lookAheadToken->type == SELF)
	{
		if (statement(root) && STATEMENTS(root))
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

bool Parser::expr(Node* root)
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
	{
		if (arithExpr(root) && 
			expr2(root))
		{
			*m_derivationFile << "expr -> arithExpr exp2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::expr2(Node* root)
{
	std::vector<TokenType> first = { EQ, GT, GEQ, LT, LEQ, NOTEQ };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == EQ || m_lookAheadToken->type == GT || m_lookAheadToken->type == GEQ ||
		m_lookAheadToken->type == LT || m_lookAheadToken->type == LEQ || m_lookAheadToken->type == NOTEQ)
	{
		if (relOp() && arithExpr(root))
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

bool Parser::relExpr(Node* root)
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* relExpr_Node = m_nodeFactory->makeNode(Type::relExpr);
	root->addChild(relExpr_Node);

	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
	{
		Node* relOp_Node = m_nodeFactory->makeNode(Type::relOp);
		relExpr_Node->addChild(relOp_Node);

		if (arithExpr(relOp_Node) && relOp() && arithExpr(relOp_Node))
		{
			*m_derivationFile << "relExpr -> arithExpr relOp arithExpr\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::arithExpr(Node* root)
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* term_Node = m_nodeFactory->makeNode();
	Node* RIGHTRECARITHEXPR_Node = m_nodeFactory->makeNode(Type::addOp);
	Node& ref_RIGHTRECARITHEXPR_Node = *RIGHTRECARITHEXPR_Node;


	if (m_lookAheadToken->type == OPENPAR || m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT || m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS || m_lookAheadToken->type == SELF)
	{
		if (term(term_Node) &&	RIGHTRECARITHEXPR(*term_Node, RIGHTRECARITHEXPR_Node))
		{
			
			root->addChild(RIGHTRECARITHEXPR_Node);
			*m_derivationFile << "arithExpr -> term RIGHTRECARITHEXPR\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::RIGHTRECARITHEXPR(Node& child, Node*& root)
{
	std::vector<TokenType> first = { MINUS, OR, PLUS, };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT , GEQ,  LT , LEQ,  NOTEQ , CLOSESQBR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == PLUS || m_lookAheadToken->type == MINUS || m_lookAheadToken->type == OR)
	{
		root->addChild(&child);
		
		Node* term_Node = m_nodeFactory->makeNode();
		Node* RIGHTRECARITHEXPR_Node = m_nodeFactory->makeNode(Type::addOp);
		Node& ref_RIGHTRECARITHEXPR_Node = *RIGHTRECARITHEXPR_Node;

		if (addOp() && term(term_Node) && RIGHTRECARITHEXPR(*term_Node, RIGHTRECARITHEXPR_Node))
		{
			root->addChild(RIGHTRECARITHEXPR_Node);
			*m_derivationFile << "RIGHTRECARITHEXPR -> addOp term arithExpr2\n";
			return true;
		}
		else return false;

	}
	else if (tokenInFollowSet(follow))
	{
		root = &child;
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

bool Parser::term(Node*& node)
{
	std::vector<TokenType> first = { OPENPAR, floatnum, intnum, NOT, id, SELF, MINUS, PLUS };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* factor_Node = m_nodeFactory->makeNode();
	Node* rightRecTerm_Node = m_nodeFactory->makeNode(Type::multiOp);

	if (m_lookAheadToken->type == OPENPAR 
		|| m_lookAheadToken->type == TokenType::floatnum
		|| m_lookAheadToken->type == TokenType::id 
		|| m_lookAheadToken->type == TokenType::SELF 
		|| m_lookAheadToken->type == TokenType::intnum
		|| m_lookAheadToken->type == NOT 
		|| m_lookAheadToken->type == PLUS
		|| m_lookAheadToken->type == MINUS)
	{			
		if (factor(factor_Node) && rightRecTerm(*factor_Node, rightRecTerm_Node))
		{
			node = *&rightRecTerm_Node;
			*m_derivationFile << "term -> factor rightRecTerm\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::rightRecTerm(Node& child, Node*& root)
{
	std::vector<TokenType> first = { MULTI, DIV, AND };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == MULTI || m_lookAheadToken->type == DIV || m_lookAheadToken->type == AND)
	{
		root->addChild(&child);

		Node* factor_Node = m_nodeFactory->makeNode();
		Node* rightRecTerm_Node = m_nodeFactory->makeNode(Type::multiOp);

		if (multOp(factor_Node) && factor(factor_Node) && rightRecTerm(*factor_Node, rightRecTerm_Node))
		{
			root->addChild(rightRecTerm_Node);
			*m_derivationFile << "rightRecTerm -> multiOp factor rightRecTerm\n";
			return true;
		}
		else return false;
	}
	else if (tokenInFollowSet(follow))
	{
		root = &child;
		*m_derivationFile << "rightRecTerm -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::factor(Node*& root)
{
	std::vector<TokenType> first = { MINUS, PLUS, id, SELF, NOT, intnum, floatnum, OPENPAR };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == MINUS || m_lookAheadToken->type == PLUS)
	{
		if (sign() && factor(root))
		{
			*m_derivationFile << "factor -> sign factor\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		root = m_nodeFactory->makeNode(idLit);
		root->token = m_lookAheadToken;

		if (IDORSELF(root) && factor2(root) && REPTVARIABLEORFUNCTIONCALL(root))
		{
			*m_derivationFile << "factor -> IDORSELF factor2 REPTVARIABLEORFUNCTIONCALL\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == NOT)
	{
		if (match(TokenType::NOT) && factor(root))
		{
			*m_derivationFile << "factor -> 'not' factor\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::intnum)
	{
		root = m_nodeFactory->makeNode(intLit);
		root->token = m_lookAheadToken;

		if (match(TokenType::intnum))
		{
			*m_derivationFile << "factor -> 'intLit'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == TokenType::floatnum)
	{
		root = m_nodeFactory->makeNode(floatLit);
		root->token = m_lookAheadToken;

		if (match(TokenType::floatnum))
		{
			*m_derivationFile << "factor -> 'floatnum'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(TokenType::OPENPAR) && arithExpr(root) && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "factor -> '(' arithExpr ')'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::factor2(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND, DOT };

	if (!skipErrors(true, first, follow)) return false;


	if (m_lookAheadToken->type == OPENSQBR)
	{


		if (indices(root))
		{

			*m_derivationFile << "factor2 -> indice\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams(root) && match(CLOSEPAR))
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

bool Parser::indice(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (match(OPENSQBR) && arithExpr(root) && match(CLOSESQBR))
		{
			*m_derivationFile << "indice -> '(' arithExpr ')'\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::indices(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { DOT, ASSIGN, CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND };

	if (!skipErrors(true, first, follow)) return false;

	Node* indice_node = m_nodeFactory->makeNode(Type::arraySizeList);

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indice(indice_node) && indices(indice_node))
		{
			root->addChild(indice_node);
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

bool Parser::REPTVARIABLEORFUNCTIONCALL(Node* root)
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		Node* dot_Node = m_nodeFactory->makeNode(Type::dot);
		root->addChild(dot_Node);

		if (idnest(dot_Node) && REPTVARIABLEORFUNCTIONCALL(dot_Node))
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

bool Parser::variable(Node* root)
{
	std::vector<TokenType> first = { id, SELF };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::id || m_lookAheadToken->type == TokenType::SELF)
	{
		root->setType(Type::idLit);

		if (IDORSELF(root) && variable2(root))
		{
			*m_derivationFile << "variable -> IDORSELF variable2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::variable2(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR, DOT, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;


	if (m_lookAheadToken->type == OPENSQBR || m_lookAheadToken->type == DOT)
	{
		Node* indices_node = m_nodeFactory->makeNode(Type::arraySizeList);
		Node* reptVariable_Node = m_nodeFactory->makeNode();

		if (indices(indices_node) && reptVariable(*indices_node, reptVariable_Node))
		{
			root->addChild(reptVariable_Node);
			*m_derivationFile << "variable2 -> indices reptVariable\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams(root) && match(CLOSEPAR) && varIdNest(root))
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

bool Parser::reptVariable(Node& child, Node* root)
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;



	if (m_lookAheadToken->type == DOT)
	{
		Node* varIdNest_Node = m_nodeFactory->makeNode();
		Node* reptVariable_Node = m_nodeFactory->makeNode();

		if (varIdNest(varIdNest_Node) && reptVariable(*varIdNest_Node, reptVariable_Node))
		{
			root->addChild(reptVariable_Node);
			*m_derivationFile << "reptVariable -> varIdNest reptVariable\n";
			return true;
		}
		else false;
	}
	else if (tokenInFollowSet(follow))
	{
		*root = child;
		*m_derivationFile << "reptVariable -> EPSILON\n";
		return true;
	}
	else return false;
}

bool Parser::varIdNest(Node* root)
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == DOT)
	{
		if (match(DOT) && match(id) && varIdNest2(root))
		{
			*m_derivationFile << "reptVariable -> '.' 'id'  varIdNest2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::varIdNest2(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, DOT};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indices(root))
		{
			*m_derivationFile << "varIdNest2 -> indices\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENPAR)
	{
		if (match(OPENPAR) && aParams(root) && match(CLOSEPAR) && varIdNest(root))
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

bool Parser::idnest(Node* root)
{
	std::vector<TokenType> first = { DOT };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::DOT)
	{
		if (match(DOT) && match(id) && idnest2(root))
		{
			*m_derivationFile << "idnest -> '.' 'id' idnest2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::idnest2(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR, OPENPAR };
	std::vector<TokenType> follow = { CLOSEPAR, SEMI, COMMA, EQ, GT, GEQ, LT, LEQ, NOTEQ, CLOSESQBR, MINUS, OR, PLUS, MULTI, DIV, AND, DOT};

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENPAR)
	{
		Node* fCall_Node = m_nodeFactory->makeNode(Type::fCall);
		root->addChild(fCall_Node);

		if (match(TokenType::OPENPAR) && aParams(fCall_Node) && match(TokenType::CLOSEPAR))
		{
			*m_derivationFile << "idnest2 -> '(' aParams ')'\n";
			return true;
		}
		else return false;
	}
	else if (m_lookAheadToken->type == OPENSQBR)
	{
		if (indices(root))
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

bool Parser::arraySize(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { };


	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (match(TokenType::OPENSQBR) && arraySize2(root))
		{
			*m_derivationFile << "arraySize -> '[' arraySize2\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::arraySize2(Node* root)
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
			root->addChild(m_nodeFactory->makeNode(Type::intLit));
			*m_derivationFile << "arraySize1 -> 'intnum' ']'\n";
			return true;
		}
	}
	else return false;
}

bool Parser::arraySizes(Node* root)
{
	std::vector<TokenType> first = { OPENSQBR };
	std::vector<TokenType> follow = { SEMI, CLOSEPAR, COMMA };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == OPENSQBR)
	{
		if (arraySize(root) && arraySizes(root))
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

bool Parser::type(Node* root)
{
	std::vector<TokenType> first = { INT, FLOAT, id };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	Node* node = m_nodeFactory->makeNode(Type::type);
	root->addChild(node);

	node->token = m_lookAheadToken;

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


bool Parser::returnType(Node* root)
{
	std::vector<TokenType> first = { FLOAT, INT, id, VOID };
	std::vector<TokenType> follow = {};

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == TokenType::INT ||
		m_lookAheadToken->type == TokenType::FLOAT ||
		m_lookAheadToken->type == TokenType::id)
	{
		if (type(root))
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

bool Parser::aParams(Node* node)
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
		if (expr(node) && REPTAPARAMS1(node))
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

bool Parser::REPTAPARAMS1(Node* root)
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (aParamsTail(root) && REPTAPARAMS1(root))
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

bool Parser::aParamsTail(Node* root)
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = {  };

	if (!skipErrors(false, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (match(TokenType::COMMA) && expr(root))
		{
			*m_derivationFile << "aParamsTail -> ',' expr\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::fParams(Node* root)
{
	std::vector<TokenType> first = { id };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	Node* fParams_Node = m_nodeFactory->makeNode(Type::fParam);
	Node* arraySizeList_Node = m_nodeFactory->makeNode(Type::arraySizeList);

	if (m_lookAheadToken->type == TokenType::id)
	{
		Node* id_node = m_nodeFactory->makeNode(Type::idLit);
		id_node->token = m_lookAheadToken;

		if (match(TokenType::id) && match(TokenType::COLON) && type(fParams_Node) && arraySizes(root) && REPTFPARAMS1(root))
		{
			fParams_Node->addChild(id_node);
			root->addChild(fParams_Node);
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

bool Parser::REPTFPARAMS1(Node* root)
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { CLOSEPAR };

	if (!skipErrors(true, first, follow)) return false;

	if (m_lookAheadToken->type == COMMA)
	{
		if (fParamsTail(root) && REPTFPARAMS1(root))
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

bool Parser::fParamsTail(Node* root)
{
	std::vector<TokenType> first = { COMMA };
	std::vector<TokenType> follow = { };

	if (!skipErrors(false, first, follow)) return false;

	Node* fParams_Node = m_nodeFactory->makeNode(Type::fParam);

	fParams_Node->token = m_lookAheadToken;

	if (m_lookAheadToken->type == COMMA)
	{
		Node* id_node = m_nodeFactory->makeNode(Type::idLit);
		id_node->token = m_lexer.peekAheadToken();

		if (match(TokenType::COMMA) && match(TokenType::id) && match(TokenType::COLON) && type(fParams_Node) && arraySizes(root))
		{
			fParams_Node->addChild(id_node);
			root->addChild(fParams_Node);
			*m_derivationFile << "fParasTail -> ',' 'id' ':' type arraySizes\n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool Parser::assignOp(Node* root)
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

bool Parser::multOp(Node* root)
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
	else if (m_lookAheadToken->type == DIV)
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

bool Parser::IDORSELF(Node* node)
{
	std::vector<TokenType> first = { id, SELF };
	std::vector<TokenType>  follow = {};

	if (!skipErrors(false, first, follow)) return false;

	node->token = m_lookAheadToken;

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

Node* Parser::getASTroot()
{
	return m_astRoot;
}

void Parser::writeAST(Node* root, int level)
{
	if (root == nullptr) {
		return;
	}

	// Print the current node's data
	// Indent according to the level of the node
	*m_ASTFile << std::string(level, ' ') << "| " << root->stringType() << std::endl;

	// Print each child node, with a '|' prefix
	for (Node* child : root->getChildren()) {
		// Recursively print each child's children
		writeAST(child, level + 2);
	}

}

