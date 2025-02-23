#pragma once
#include <string>
#include <stdarg.h>


/////////////////////////////////////////////////////////////////////

class Node
{
private:
	std::string type;
public:
	std::vector<Node*> children;
	std::vector<Node*> siblings;

	Token* token = nullptr;
	virtual std::string getType() = 0;


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
	void printNodeTokenLexem()
	{
		if (this->token == nullptr)
		{
			std::cout << "ERROR: Node has no token attached...... " << std::endl;
		}
		else 
		{
			std::cout << "Node Token Lexem: " + this->token->lexem << std::endl;
		}
	}; 



};

/////////////////////////////////////////////////////////////////////

class null_Node : public Node
{
public:
	std::string getType() { return "NULL"; }

};

/////////////////////////////////////////////////////////////////////

class prog_Node : public Node
{
public:
	std::string getType() { return "prog"; }

};

/////////////////////////////////////////////////////////////////////

class classDeclList_Node : public Node
{
public:
	std::string getType() { return "classDeclList"; }

};

/////////////////////////////////////////////////////////////////////

class idLit_Node : public Node
{
public:
	std::string getType() {return "id";}

};

/////////////////////////////////////////////////////////////////////

class type_Node : public Node
{
public:
	std::string getType() { return "type"; };

};

/////////////////////////////////////////////////////////////////////

class intLit_Node : public Node
{
public:
	std::string getType() { return "intLit"; }

};

/////////////////////////////////////////////////////////////////////

class floatLit_Node : public Node
{
public:
	std::string getType() { return "floatLit"; }

};

/////////////////////////////////////////////////////////////////////

class relOp_Node : public Node
{
public:
	std::string getType() { return "relOp"; }

};

/////////////////////////////////////////////////////////////////////

class addOp_Node : public Node
{
public:
	std::string getType() { return "addOp"; }

};

/////////////////////////////////////////////////////////////////////

class multiOp_Node : public Node
{
public:
	std::string getType() { return "multiOp"; }

};

/////////////////////////////////////////////////////////////////////

class assignOp_Node : public Node
{
public:
	std::string getType() { return "assignOp"; }

};

/////////////////////////////////////////////////////////////////////

enum Type {
	
	// Terminals
	idLit, intLit, floatLit, type, relOp, multiOp, assignOp, addOp,

	prog,
	classDeclList, funcDefList, implDefList,
	classDecl, funcDef, ImpleDef,
	inheritList, memberList,
	memberDecl, FuncDecl, varDecl,
	dimList, fParamsList, 
	fParam, aParams,

	statBlock, 
	statOrVarDecl, 


	_not, sign, plus, minus, 
	term, factor, 
	dot, dotParams,
	assignStat, ifStat, whileStat, readStat, writeStat, returnStat,
	relExpr, airthExpr, 
	expr,


};

/////////////////////////////////////////////////////////////////////

class NodeFactory
{
public:
	NodeFactory()
	{
		std::cout << "Node Factory created....." << std::endl;
	}

	/////////////////////////////////////////////////////////////////////

	Node* makeNode()
	{
		return new null_Node();
	}

	/////////////////////////////////////////////////////////////////////

	Node* makeNode(Type t)
	{
		switch (t)
		{
		case idLit:
			return new idLit_Node();
		case intLit:
			return new intLit_Node();
		case floatLit:
			return new floatLit_Node();
		case relOp:
			return new relOp_Node();
		case addOp:
			return new addOp_Node();
		case multiOp:
			return new multiOp_Node();
		case assignOp:
			return new assignOp_Node();

		case prog:
			return new prog_Node();
		case classDeclList:
			return new classDeclList_Node();

		default: 
			std::cout << "Error: Invalid Type past to makeNode()....." << std::endl;
			return nullptr;
		}
	}

	/////////////////////////////////////////////////////////////////////
	
	Node* makeSubtree(Node* op, int count, ... )
	{
		va_list args;

		va_start(args, count);
		int i;

		for (i = 0; i < count; i++)
		{
			Node* temp = va_arg(args, Node*);
			op->children.push_back(temp);
		}
		va_end(args);
		
		return op;
	}

	/////////////////////////////////////////////////////////////////////

	void makeSiblings(int count, Node*...)
	{

	}

};