#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

#include "Node.h"

class TypeCheckingVisitor : public Visitor
{
public:
    TypeCheckingVisitor(std::ofstream* errors);
	 void visit(Node& node) override;
	 void visit(prog_Node& node) override;

	 void visit(classDecl_Node& node) override;
	 void visit(funcDef_Node& node) override;
	 void visit(impleDef_Node& node) override;

	 void visit(memDeclAttrib_Node& node) override;
	 void visit(memDeclFunc_Node& node) override;
	 void visit(fParam_Node& node) override;
	 void visit(statBlock_Node& node) override;

	 void visit(inheritList_Node& node) override;

	 void visit(varDecl_Node& node) override;

	 // Operators
	 void visit(assignOp_Node& node) override;
	 void visit(multiOp_Node& node) override;


	// Terminals
	 void visit(idLit_Node& node) override;
	 void visit(intLit_Node& node) override;
	 void visit(floatLit_Node& node) override;
	 void visit(type_Node& node) override;

private:
    std::ofstream* m_errors;



	// Inherited via Visitor
	virtual void visit(assignStat_Node& node) override;


	// Inherited via Visitor
	virtual void visit(addOp_Node& node) override;

};

