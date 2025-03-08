#pragma once
#include <string>
#include <vector>
#include <iostream>


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
	memDecl, 
	funcDecl, 
	varDecl,
	dimList, 
	fParamsList,
	fParam, 
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
	Token* token = nullptr;

	std::vector<Node*> children;

public:

	Node* parent = nullptr; 
	std::vector<Node*> siblings;

	// Constructors 
	Node(Type t) : m_type(t)
	{ };

	Node() {};

	Node(Token* token, Type type)
	{
		this->m_type = type;
		this->token = token;
	}

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

	std::vector<Node*> getChildren()
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

	void printSiblings()
	{
		std::cout << "-------- \n" << std::endl;

		if (siblings.empty())
		{
			std::cout << "Has no Siblings" << std::endl;
		}
		else
		{
			for (auto i : siblings)
			{
				std::cout << i->getType() << std::endl;
			}
		}
	};

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
			return "floatLit";
		case Type::intLit:
			return "intLit";
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
		case Type::memDecl:
			return "memDecl";
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
		case Type::ifStat:
			return "ifStat";




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

/////////////////////////////////////////////////////////////////////

class null_Node : public Node
{
	
};

/////////////////////////////////////////////////////////////////////

class prog_Node : public Node
{
public:
	prog_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class classDeclList_Node : public Node
{
public:
	classDeclList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class classDecl_Node : public Node
{
public:
	classDecl_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class implDefList_Node : public Node
{
public:
	implDefList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class impleDef_Node : public Node
{
public:
	impleDef_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class funcDefList_Node : public Node
{
public:
	funcDefList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class funcDecl_Node : public Node
{
public:
	funcDecl_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class funcDef_Node : public Node
{
public:
	funcDef_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class inheritList_Node : public Node
{
public:
	inheritList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class memDeclList_Node : public Node
{
public:
	memDeclList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class memDecl_Node : public Node
{
public:
	memDecl_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class fParamsList_Node : public Node
{
public:
	fParamsList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class fParam_Node : public Node
{
public:
	fParam_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class arraySizeList_Node : public Node
{
public:
	arraySizeList_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class statBlock_Node : public Node
{
public:
	statBlock_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class returnStat_Node : public Node
{
public:
	returnStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class whileStat_Node : public Node
{
public:
	whileStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class writeStat_Node : public Node
{
public:
	writeStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class readStat_Node : public Node
{
public:
	readStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class ifStat_Node : public Node
{
public:
	ifStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class expr_Node : public Node
{
public:
	expr_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class relExpr_Node : public Node
{
public:
	relExpr_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class varDecl_Node : public Node
{
public:
	varDecl_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class assignStat_Node : public Node
{
public:
	assignStat_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////




class idLit_Node : public Node
{
public:
	idLit_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class type_Node : public Node
{
public:
	type_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class intLit_Node : public Node
{
public:
	intLit_Node(Type t) : Node(t)
	{ }

};

/////////////////////////////////////////////////////////////////////

class floatLit_Node : public Node
{
public:
	floatLit_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class relOp_Node : public Node
{
public:
	relOp_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class addOp_Node : public Node
{
public:
	addOp_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

class multiOp_Node : public Node
{
public:
	multiOp_Node(Type t) : Node(t)
	{ }
};

/////////////////////////////////////////////////////////////////////

