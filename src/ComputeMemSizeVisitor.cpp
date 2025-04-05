#include "ComputeMemSizeVisitor.h"

ComputeMemSizeVisitor::ComputeMemSizeVisitor()
{
}

int ComputeMemSizeVisitor::sizeOfTypeNode(Node* node)
{
	int size = 0;
	if (node->getType()  == Type::intLit)
	{
		size = 4;
	}

	return size;
}

void ComputeMemSizeVisitor::visit(Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(prog_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(classDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(funcDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(impleDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(memDeclAttrib_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(memDeclFunc_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(fParam_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(statBlock_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(inheritList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(varDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(node.getChildren()[1]);
}

void ComputeMemSizeVisitor::visit(assignStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(assignOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(multiOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(addOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(idLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(intLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(&node);
}

void ComputeMemSizeVisitor::visit(floatLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(type_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(funcDefList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void ComputeMemSizeVisitor::visit(writeStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}
