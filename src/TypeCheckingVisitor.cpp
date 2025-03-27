#include "TypeCheckingVisitor.h"


TypeCheckingVisitor::TypeCheckingVisitor(std::ofstream* errors)
{
	m_errors = errors;
}

void TypeCheckingVisitor::visit(Node& node)
{
}

void TypeCheckingVisitor::visit(prog_Node& node)
{
}

void TypeCheckingVisitor::visit(classDecl_Node& node)
{
}

void TypeCheckingVisitor::visit(funcDef_Node& node)
{ 
}

void TypeCheckingVisitor::visit(impleDef_Node& node)
{
}

void TypeCheckingVisitor::visit(memDeclAttrib_Node& node)
{
}

void TypeCheckingVisitor::visit(memDeclFunc_Node& node)
{
}

void TypeCheckingVisitor::visit(fParam_Node& node)
{
}

void TypeCheckingVisitor::visit(statBlock_Node& node)
{
}

void TypeCheckingVisitor::visit(inheritList_Node& node)
{
}

void TypeCheckingVisitor::visit(varDecl_Node& node)
{
}

void TypeCheckingVisitor::visit(assignOp_Node& node)
{


}

void TypeCheckingVisitor::visit(multiOp_Node& node)
{
	std::string leftOperandType = node.getChildren()[0]->token->convertTokenTypeToString();

}

void TypeCheckingVisitor::visit(idLit_Node& node)
{
}

void TypeCheckingVisitor::visit(intLit_Node& node)
{
}

void TypeCheckingVisitor::visit(floatLit_Node& node)
{
}

void TypeCheckingVisitor::visit(type_Node& node)
{

}

void TypeCheckingVisitor::visit(assignStat_Node& node)
{
}
