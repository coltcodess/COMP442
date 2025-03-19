#pragma once
#include "Node_Base.h"

class Node; 
class prog_Node;
class classDeclList_Node;

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

	virtual void visit(idLit_Node& node) = 0;
	virtual void visit(intLit_Node& node) = 0;
	virtual void visit(floatLit_Node& node) = 0;
	virtual void visit(type_Node& node) = 0;
};