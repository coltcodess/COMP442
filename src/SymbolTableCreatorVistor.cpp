#include "SymbolTableCreatorVistor.h"

SymbolTableCreatorVistor::SymbolTableCreatorVistor(std::ofstream* output, std::ofstream* errors)
{
	m_output = output;
	m_errors = errors;
}

void SymbolTableCreatorVistor::visit(Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

//////////////////
// Non-Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(prog_Node& node)
{
	node.m_symbolTable = new SymbolTable(0, "global", nullptr);
	bool mainIncluded = false;

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

}

void SymbolTableCreatorVistor::visit(funcDefList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(writeStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(classDecl_Node& node)
{
	std::string className = node.token->lexem;
	SymbolTable* localTable = new SymbolTable(1, className, node.m_symbolTable);
	node.m_symbolEntry = new SymbolTableEntry(className, localTable);

	node.m_symbolTable->appendEntry(node.m_symbolEntry);

	node.m_symbolTable = localTable;
	
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(funcDef_Node& node)
{
	std::string funcName;
	if (node.parent->token != nullptr)
	{
		funcName = node.parent->token->lexem + "::" + node.token->lexem;

	}
	else
	{
		funcName = node.token->lexem;
	}
	std::vector<SymbolTableEntry*>* entries = new std::vector<SymbolTableEntry*>();
	SymbolTable* localTable = new SymbolTable(1, funcName, node.m_symbolTable);
	node.m_symbolEntry = new SymbolTableEntry(funcName, *entries, localTable );

	node.m_symbolTable->appendEntry(node.m_symbolEntry);
	node.m_symbolTable = localTable;
	
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(impleDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(inheritList_Node& node)
{

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

}

void SymbolTableCreatorVistor::visit(memDeclAttrib_Node& node)
{

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(memDeclFunc_Node& node)
{


	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(statBlock_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(fParam_Node& node)
{

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

}

void SymbolTableCreatorVistor::visit(varDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	std::string name = node.token->lexem;
	std::string type = node.token->convertTokenTypeToString();

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_variable, node.getChildren()[1]->stringType());
	node.m_symbolTable->appendEntry(node.m_symbolEntry);


}

void SymbolTableCreatorVistor::visit(assignOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}


//////////////////
// Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(idLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	node.moonVarName = node.token->lexem;

}

void SymbolTableCreatorVistor::visit(intLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	std::string tempVarName = this->getNewTempVarName();
	node.moonVarName = tempVarName;
	node.setType(Type::intLit);
	std::string type = "int";
	node.m_symbolEntry = new SymbolTableEntry(tempVarName, Kind::_variable, type, nullptr);
	node.m_symbolTable->appendEntry(node.m_symbolEntry);

}

void SymbolTableCreatorVistor::visit(floatLit_Node& node)
{
	node.m_symbolEntry = new SymbolTableEntry(node.token->lexem);
	node.m_symbolEntry->type = Type::floatLit;

}

void SymbolTableCreatorVistor::visit(type_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

// Good
void SymbolTableCreatorVistor::visit(assignStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(addOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	std::string tempVar = this->getNewTempVarName();
	node.moonVarName = tempVar;
	node.setType(node.getChildren()[0]->getType());
	node.m_symbolEntry = new SymbolTableEntry(node.moonVarName, Kind::_variable, node.getChildren()[0]->stringType());
	node.m_symbolTable->appendEntry(node.m_symbolEntry);
}

void SymbolTableCreatorVistor::visit(multiOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	std::string tempVar = this->getNewTempVarName();
	node.moonVarName = tempVar;
	node.m_symbolEntry = new SymbolTableEntry(node.moonVarName, Kind::_variable, node.getChildren()[0]->stringType());
	node.m_symbolTable->appendEntry(node.m_symbolEntry);
}

std::string SymbolTableCreatorVistor::getNewTempVarName()
{
	tempVarNum++;
	return "t" + std::to_string(tempVarNum);
}





