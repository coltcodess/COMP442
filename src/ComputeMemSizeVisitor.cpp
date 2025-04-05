#include "ComputeMemSizeVisitor.h"

ComputeMemSizeVisitor::ComputeMemSizeVisitor()
{
}

int ComputeMemSizeVisitor::sizeOfTypeNode(Node* node)
{
	int size = 0;

	if (node->m_symbolEntry->type.compare("int") == 0)
	{
		size = 4;
	}
	else if (node->m_symbolEntry->type.compare("float") == 0)
	{
		size = 8;
	}
	else if (node->m_symbolEntry->type.compare("id") == 0)
	{
		for (SymbolTableEntry* entry : node->m_symbolTable->upperTable->getEntries())
		{
			if (entry->name.compare(node->getChildren()[1]->token->lexem) == 0)
			{
				size = abs(entry->link->m_tableOffset);
			}
		}
	}

	SymbolTableEntry* entry = node->m_symbolEntry;
	if (entry != nullptr && !entry->arrayList.empty())
	{
		for (int i : entry->arrayList)
			size *= i;
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

	for (SymbolTableEntry* entry : node.m_symbolTable->getEntries())
	{
		entry->m_entryOffset = node.m_symbolTable->m_tableOffset - entry->m_entrySize;
		node.m_symbolTable->m_tableOffset -= entry->m_entrySize;
	}
}

void ComputeMemSizeVisitor::visit(funcDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	for (SymbolTableEntry* entry : node.m_symbolTable->getEntries())
	{
		entry->m_entryOffset = node.m_symbolTable->m_tableOffset - entry->m_entrySize;
		node.m_symbolTable->m_tableOffset -= entry->m_entrySize;
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

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(&node);
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

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(&node);
}

void ComputeMemSizeVisitor::visit(addOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		
		child->accept(*this);
	}

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(&node);
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

	node.m_symbolEntry->m_entrySize = this->sizeOfTypeNode(&node);
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

void ComputeMemSizeVisitor::visit(ifStat_Node& node)
{
}

void ComputeMemSizeVisitor::visit(relExpr_Node& node)
{
}

void ComputeMemSizeVisitor::visit(fCall_Node& node)
{
}
