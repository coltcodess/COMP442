#pragma once
#include "Node_Base.h"

class Node; 
class prog_Node;
class classDeclList_Node;
class funcDefList_Node;
class implDefList_Node;
class classDecl_Node;
class funcDef_Node;
class impleDef_Node;

class varDecl_Node;

class idLit_Node;
class intLit_Node;
class floatLit_Node;
class type_Node;

class Visitor
{
public:
	Visitor() {};
	virtual void visit(Node& node) = 0;
	virtual void visit(prog_Node& node) = 0;
	virtual void visit(classDeclList_Node& node) = 0;
	virtual void visit(funcDefList_Node& node) = 0;
	virtual void visit(implDefList_Node& node) = 0;
	

	virtual void visit(classDecl_Node& node) = 0;
	virtual void visit(funcDef_Node& node) = 0;
	virtual void visit(impleDef_Node& node) = 0;

	virtual void visit(varDecl_Node& node) = 0;

	// Terminals
	virtual void visit(idLit_Node& node) = 0;
	virtual void visit(intLit_Node& node) = 0;
	virtual void visit(floatLit_Node& node) = 0;
	virtual void visit(type_Node& node) = 0;

};