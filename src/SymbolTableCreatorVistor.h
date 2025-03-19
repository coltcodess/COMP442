#pragma once
#include <fstream>
#include <iostream>
#include <sstream>

#include "Node.h"

class SymbolTableCreatorVistor : public Visitor
{
public:
	SymbolTableCreatorVistor(std::ofstream* output);
	virtual ~SymbolTableCreatorVistor() {};

	void visit(Node& node) override;
	void visit(prog_Node& node) override;
	void visit(classDeclList_Node& node) override;


private: 
	std::ofstream* m_output;

};

