#pragma once
#include "Node_Base.h"

class Node; 
class prog_Node;
class classDeclList_Node;

class Visitor
{
public:
	Visitor() {};
	virtual void visit(Node& node) = 0;
	virtual void visit(prog_Node& node) = 0;
	virtual void visit(classDeclList_Node& node) = 0;

};