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
	void visit(funcDefList_Node& node) override;
	void visit(implDefList_Node& node) override;
	void visit(classDecl_Node& node) override;
	void visit(funcDef_Node& node) override;
	void visit(impleDef_Node& node) override;
	void visit(inheritList_Node& node) override;

	void visit(varDecl_Node& node) override;

	void visit(idLit_Node& node) override;
	void visit(intLit_Node& node) override;
	void visit(floatLit_Node& node) override;
	void visit(type_Node& node) override;

	void print();



private: 
	std::ofstream* m_output;

};

