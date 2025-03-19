#include "SymbolTableCreatorVistor.h"

SymbolTableCreatorVistor::SymbolTableCreatorVistor(std::ofstream* output)
{
	
}

void SymbolTableCreatorVistor::visit(Node& node)
{
	std::cout << "Visited base" << std::endl;
}

void SymbolTableCreatorVistor::visit(prog_Node& node)
{
	std::cout << "Visited prog" << std::endl;
	
}

void SymbolTableCreatorVistor::visit(classDeclList_Node& node)
{
	std::cout << "Visited classDeclList_Node" << std::endl;

}

