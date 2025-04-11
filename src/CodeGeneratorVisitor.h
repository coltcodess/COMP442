#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <format>
#include "Node.h"

class CodeGeneratorVisitor : public Visitor
{
public:
	CodeGeneratorVisitor(std::ofstream* output);
	int tempVarNum = 0;
	std::vector<std::string> registerPool;

	std::string moonExecCode; 
	std::string moonDataCode;

private:
	std::ofstream* m_output;

	// Inherited via Visitor
	virtual void visit(Node& node) override;

	virtual void visit(prog_Node& node) override;

	virtual void visit(classDecl_Node& node) override;

	virtual void visit(funcDef_Node& node) override;

	virtual void visit(funcDefList_Node& node) override;

	virtual void visit(impleDef_Node& node) override;

	virtual void visit(memDeclAttrib_Node& node) override;

	virtual void visit(memDeclFunc_Node& node) override;

	virtual void visit(fParam_Node& node) override;

	virtual void visit(statBlock_Node& node) override;

	virtual void visit(inheritList_Node& node) override;

	virtual void visit(varDecl_Node& node) override;

	virtual void visit(assignOp_Node& node) override;

	virtual void visit(multiOp_Node& node) override;

	virtual void visit(idLit_Node& node) override;

	virtual void visit(intLit_Node& node) override;

	virtual void visit(floatLit_Node& node) override;

	virtual void visit(type_Node& node) override;

	virtual void visit(addOp_Node& node) override;

	virtual void visit(assignStat_Node& node) override;

	virtual void visit(writeStat_Node& node) override;

	virtual void visit(ifStat_Node& node) override;

	virtual void visit(whileStat_Node& node) override;

	virtual void visit(relOp_Node& node) override;

	virtual void visit(fCall_Node& node) override;

	virtual void visit(returnStat_Node& node) override;
};

