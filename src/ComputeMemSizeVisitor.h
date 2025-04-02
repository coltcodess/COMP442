#pragma once
#include "Visitor.h"
#include "Node.h"

class ComputeMemSizeVisitor :
    public Visitor
{
public:
    ComputeMemSizeVisitor();

    // Inherited via Visitor
    virtual void visit(Node& node) override;
    virtual void visit(prog_Node& node) override;
    virtual void visit(classDecl_Node& node) override;
    virtual void visit(funcDef_Node& node) override;
    virtual void visit(impleDef_Node& node) override;
    virtual void visit(memDeclAttrib_Node& node) override;
    virtual void visit(memDeclFunc_Node& node) override;
    virtual void visit(fParam_Node& node) override;
    virtual void visit(statBlock_Node& node) override;
    virtual void visit(inheritList_Node& node) override;
    virtual void visit(varDecl_Node& node) override;
    virtual void visit(assignStat_Node& node) override;
    virtual void visit(assignOp_Node& node) override;
    virtual void visit(multiOp_Node& node) override;
    virtual void visit(addOp_Node& node) override;
    virtual void visit(idLit_Node& node) override;
    virtual void visit(intLit_Node& node) override;
    virtual void visit(floatLit_Node& node) override;
    virtual void visit(type_Node& node) override;
};

