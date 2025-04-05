#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Token.h"
#include "SymbolTable.h"

class Visitor;

enum Type {

	// Terminals
	idLit,
	intLit = 1,
	floatLit,
	type,
	relOp,
	multiOp,
	assignOp,
	addOp,
	prog,
	classDeclList,
	funcDefList,
	implDefList,
	classDecl,
	funcDef,
	impleDef,
	inheritList,
	memDeclList,
	memDeclFunc,
	memDeclAttrib,
	funcDecl,
	varDecl,
	dimList,
	fParamsList,
	fParam,
	fCall,
	aParams,
	arraySizeList,

	statBlock,
	statOrVarDecl,

	_not,
	sign,
	plus,
	minus,
	term,
	factor,
	dot,
	dotParams,
	dataMem,
	assignStat,
	ifStat,
	whileStat,
	readStat,
	writeStat,
	returnStat,
	relExpr,
	arithExpr,
	expr,

	none,
	error,

};



class Node
{
private:
	Type m_type = Type::none;

	std::vector<Node*> children;

public:

	Token* token;
	Node* parent = nullptr;
	SymbolTable* m_symbolTable = nullptr;
	SymbolTableEntry* m_symbolEntry = nullptr;


	std::string moonVarName;

	std::string localRegister = "";
	std::string leftChildRegister = "";
	std::string rightChildRegister = "";
	static SymbolTable* m_GLOBAL_symbolTable;

	// Constructors 
	Node(Type t) : m_type(t)
	{	};

	Node() {};

	Node(Token* token, Type type)
	{
		this->m_type = type;
		this->token = token;
	}

	virtual ~Node() {};

	virtual void accept(Visitor& visitor) = 0;

	void addChild(Node* child)
	{
		child->parent = this;
		children.push_back(child);
	}

	bool hasChild(Type type)
	{
		for (auto i : children)
		{
			if (i->m_type == type)
			{
				return true;
			}
		}
		return false;
	}

	Node* getChild(Type type)
	{
		for (auto i : children)
		{
			if (i->m_type == type)
			{
				return i;
			}
		}
		return nullptr;
	}

	void removeChild(Node* child)
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i] == child)
			{
				this->children.erase(children.begin() + i);
			}
		}

	}

	const std::vector<Node*> getChildren()
	{
		return this->children;
	}

	Type getType()
	{
		return this->m_type;
	}

	void setType(Type type)
	{
		m_type = type;
	}

	bool hasChild(Node* node)
	{
		return node->children.empty();
	}

	Token* getToken()
	{
		return this->token;
	}

	std::string getData()
	{
		return "";
	}

	void printChildren()
	{
		std::cout << "-------- \n" << std::endl;
		std::cout << "Parent: " + this->getType() << std::endl;

		if (children.empty())
		{
			std::cout << "Has no children" << std::endl;
		}
		else
		{
			for (auto i : children)
			{
				std::cout << i->getType() << std::endl;
			}
		}
	};

	void transferChildren(Node* node)
	{
		for (auto i : this->getChildren())
		{
			i->parent = node;
			node->addChild(i);
		}

		this->parent = node->parent;
	}


	const std::string stringType()
	{
		switch (this->m_type)
		{
		case Type::prog:
			return "prog";
		case Type::classDecl:
			return  "classDecl";
		case Type::implDefList:
			return  "implDefList";
		case Type::impleDef:
			return  "impleDef";
		case Type::funcDefList:
			return  "funcDefList";
		case Type::funcDecl:
			return  "funcDecl";
		case Type::funcDef:
			return  "funcDef";
		case Type::classDeclList:
			return "classDeclList";
		case Type::floatLit:
			return "float";
		case Type::intLit:
			return "int";
		case Type::type:
			return "type";
		case Type::inheritList:
			return "inheritList";
		case Type::idLit:
			return "idLit";
		case Type::expr:
			return "expr";
		case Type::addOp:
			return "addOp";
		case Type::multiOp:
			return "multiOp";
		case Type::memDeclList:
			return "memDeclList";
		case Type::memDeclFunc:
			return "memDeclFunc";
		case Type::memDeclAttrib:
			return "memDeclAttrib";
		case Type::fParamsList:
			return "fParamsList";
		case Type::fParam:
			return "fParam";
		case Type::arraySizeList:
			return "arraySizeList";
		case Type::statBlock:
			return "statBlock";
		case Type::returnStat:
			return "returnStat";
		case Type::varDecl:
			return "varDecl";
		case Type::assignStat:
			return "assignStat";
		case Type::whileStat:
			return "whileStat";
		case Type::writeStat:
			return "writeStat";
		case Type::readStat:
			return "readStat";
		case Type::relExpr:
			return "relExpr";
		case Type::relOp:
			return "relOp";
		case Type::assignOp:
			return "assignOp";
		case Type::ifStat:
			return "ifStat";
		case Type::fCall:
			return "fCall";
		case Type::aParams:
			return "aParams";
		case Type::dot:
			return "dot";
		case Type::dataMem:
			return "dataMem";

		case Type::error:
			return "error";



		default:
			return "+++";
		}
	}

	friend std::ostream& operator<< (std::ostream& stream, const Node& node)
	{
		std::string parent;

		if (node.parent == nullptr)
		{
			parent = "-";
		}
		else
		{
			parent = std::to_string(node.parent->m_type);
		}

		return stream << "Node [type: " + std::to_string(node.m_type) + " | parent " + parent + " | children " + std::to_string(node.children.size()) + " ]";
	}

	

};
