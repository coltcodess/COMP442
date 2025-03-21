#include "SymbolTableCreatorVistor.h"

SymbolTableCreatorVistor::SymbolTableCreatorVistor(std::ofstream* output)
{
	m_output = output;
}

void SymbolTableCreatorVistor::visit(Node& node)
{
	std::cout << "Visited base" << std::endl;
}

//////////////////
// Non-Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(prog_Node& node)
{
	std::cout << "Visited prog" << std::endl;
	node.m_symbolTable = new SymbolTable("global");
	*m_output << " || Global -------------------------------------------------- ||" << std::endl;



	node.m_symbolTable->appendEntry(SymbolTableEntry("program", Kind::_function));

	for (auto i : node.m_symbolTable->getEntries())
	{
		*m_output << " ||   " + i.name + " | " << std::endl;
	}
}

void SymbolTableCreatorVistor::visit(classDeclList_Node& node)
{
	std::cout << "Visited classDeclList_Node" << std::endl;



}

void SymbolTableCreatorVistor::visit(funcDefList_Node& node)
{
	std::cout << "Visited funcDefList_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(implDefList_Node& node)
{
	std::cout << "Visited implDefList_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(classDecl_Node& node)
{
	std::cout << "Visited classDecl_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(funcDef_Node& node)
{
	std::cout << "Visited funcDef_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(impleDef_Node& node)
{
	std::cout << "Visited impleDef_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(varDecl_Node& node)
{
	std::cout << "Visited varDecl_Node" << std::endl;
}

//////////////////
// Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(idLit_Node& node)
{
	std::cout << "Visited idLit_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(intLit_Node& node)
{
	std::cout << "Visited intLit_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(floatLit_Node& node)
{
	std::cout << "Visited floatLit_Node" << std::endl;
}

void SymbolTableCreatorVistor::visit(type_Node& node)
{
	std::cout << "Visited type_Node" << std::endl;
}



