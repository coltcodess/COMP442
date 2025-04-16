#include "TypeCheckingVisitor.h"


TypeCheckingVisitor::TypeCheckingVisitor(std::ofstream* errors)
{
	m_errors = errors;
}

void TypeCheckingVisitor::visit(Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(prog_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(classDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(funcDef_Node& node)
{ 
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(impleDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(memDeclAttrib_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(memDeclFunc_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(fParam_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(statBlock_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(inheritList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(varDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}


}

void TypeCheckingVisitor::visit(assignOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}



}

void TypeCheckingVisitor::visit(multiOp_Node& node)
{
	std::string leftOperandType = node.getChildren()[0]->token->convertTokenTypeToString();
	std::string rightOperandType = node.getChildren()[1]->token->convertTokenTypeToString();

	if (leftOperandType.compare(rightOperandType) != 0)
	{
		*m_errors << "Type error between - " + node.getChildren()[0]->token->lexem + " and " + node.getChildren()[1]->token->lexem + "\n";
	}

	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

}

void TypeCheckingVisitor::visit(idLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(intLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(floatLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(type_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(assignStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(addOp_Node& node)
{
	std::string leftOperandType = node.getChildren()[0]->token->convertTokenTypeToString();
	std::string rightOperandType = node.getChildren()[1]->token->convertTokenTypeToString();

	if(leftOperandType.compare(rightOperandType) != 0)
	{
		*m_errors << "Type error between - " + node.getChildren()[0]->token->lexem + " and " + node.getChildren()[1]->token->lexem + "\n";
	}

	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(funcDefList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(fCall_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	if (node.token->lexem == "gi")
	{

	}

}

void TypeCheckingVisitor::visit(returnStat_Node& node)
{
	Node* funcDef = node.parent->parent;
	std::string returnType = node.getChildren()[0]->token->convertTokenTypeToString();

	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}


	
}

void TypeCheckingVisitor::visit(relOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(writeStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(ifStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(whileStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void TypeCheckingVisitor::visit(dot_Node& node)
{
	std::string funcCallName = node.getChildren()[0]->token->lexem;
	SymbolTableEntry* funcCallEntry = node.m_symbolTable->lookupName(funcCallName);

	if (funcCallEntry->type == "int" || funcCallEntry->type == "float")
	{
		*m_errors << " '.' operator used on non - class type - " + funcCallName + "\n";
	}


	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	
}
