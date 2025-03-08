#pragma once
#include <string>
#include <stdarg.h>
#include "Node.h"

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
			return new idLit_Node(t);
		case intLit:
			return new intLit_Node(t);
		case floatLit:
			return new floatLit_Node(t);
		case type:
			return new type_Node(t);
		case relOp:
			return new relOp_Node(t);
		case addOp:
			return new addOp_Node(t);
		case multiOp:
			return new multiOp_Node(t);
		case prog:
			return new prog_Node(t);
		case classDeclList:
			return new classDeclList_Node(t);
		case classDecl:
			return new classDecl_Node(t);
		case implDefList:
			return new implDefList_Node(t);
		case funcDefList:
			return new funcDefList_Node(t);
		case funcDecl:
			return new funcDecl_Node(t);
		case funcDef:
			return new funcDef_Node(t);
		case inheritList:
			return new inheritList_Node(t);
		case memDeclList:
			return new memDeclList_Node(t);
		case memDecl:
			return new memDecl_Node(t);
		case fParamsList:
			return new fParamsList_Node(t);
		case fParam:
			return new fParam_Node(t);
		case arraySizeList:
			return new arraySizeList_Node(t);
		case statBlock:
			return new statBlock_Node(t);
		case returnStat:
			return new returnStat_Node(t);
		case whileStat:
			return new whileStat_Node(t);
		case writeStat:
			return new writeStat_Node(t);
		case expr:
			return new expr_Node(t);
		case varDecl:
			return new varDecl_Node(t);
		case assignStat:
			return new varDecl_Node(t);
		case relExpr:
			return new relExpr_Node(t);



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
			op->addChild(temp);
		}
		va_end(args);
		
		return op;
	}

	/////////////////////////////////////////////////////////////////////

	void makeSiblings(int count, Node*...)
	{


	}

};