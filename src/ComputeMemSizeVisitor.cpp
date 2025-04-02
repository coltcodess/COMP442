#include "ComputeMemSizeVisitor.h"

ComputeMemSizeVisitor::ComputeMemSizeVisitor()
{
}

void ComputeMemSizeVisitor::visit(Node& node)
{
}

void ComputeMemSizeVisitor::visit(prog_Node& node)
{
}

void ComputeMemSizeVisitor::visit(classDecl_Node& node)
{
}

void ComputeMemSizeVisitor::visit(funcDef_Node& node)
{

}

void ComputeMemSizeVisitor::visit(impleDef_Node& node)
{
}

void ComputeMemSizeVisitor::visit(memDeclAttrib_Node& node)
{
}

void ComputeMemSizeVisitor::visit(memDeclFunc_Node& node)
{
}

void ComputeMemSizeVisitor::visit(fParam_Node& node)
{
}

void ComputeMemSizeVisitor::visit(statBlock_Node& node)
{
}

void ComputeMemSizeVisitor::visit(inheritList_Node& node)
{
}

void ComputeMemSizeVisitor::visit(varDecl_Node& node)
{
	node.m_symbolTable->getEntryByName(node.token->lexem)->m_entrySize = 4;
	node.m_symbolEntry->m_entrySize = 4;
}

void ComputeMemSizeVisitor::visit(assignStat_Node& node)
{
}

void ComputeMemSizeVisitor::visit(assignOp_Node& node)
{

}

void ComputeMemSizeVisitor::visit(multiOp_Node& node)
{
}

void ComputeMemSizeVisitor::visit(addOp_Node& node)
{

}

void ComputeMemSizeVisitor::visit(idLit_Node& node)
{
	
}

void ComputeMemSizeVisitor::visit(intLit_Node& node)
{

}

void ComputeMemSizeVisitor::visit(floatLit_Node& node)
{
}

void ComputeMemSizeVisitor::visit(type_Node& node)
{
}
