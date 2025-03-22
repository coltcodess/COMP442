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

	*m_output << " || table: " + node.m_symbolTable->getName() << std::endl;

	*m_output << " -------------------------------- " << std::endl;


	// ClassDeclList
	for (Node* i : node.getChildren()[0]->getChildren())
	{
		node.m_symbolTable->appendEntry(*i->m_symbolEntry);
		*m_output << " ||   " + i->m_symbolEntry->name + " | " + "id" << std::endl;
		
		*m_output << " -------------------------------- " << std::endl;

		*m_output << "   ||   table: " + i->m_symbolTable->getName() << std::endl;

		
		if (i->getChildren()[1]->m_symbolEntry != nullptr)
		{
			*m_output << "     ||   inherit | " << std::endl;
			for (auto j : i->getChildren()[1]->getChildren())
			{
				{
					node.m_symbolTable->appendEntry(*j->m_symbolEntry);
					*m_output << "     ||   data | id | type" << std::endl;
				}
			}
		}
		else
		{
			*m_output << "     ||   inherit | none" << std::endl;
		}


		


		*m_output << "     ||   function | " << std::endl;


	}

	//FuncDeclList
	for (auto i : node.getChildren()[2]->getChildren())
	{
		node.m_symbolTable->appendEntry(*i->m_symbolEntry);
		*m_output << " ||   " + i->m_symbolEntry->name + " | " << std::endl;
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

	node.m_symbolTable = new SymbolTable("className");

	std::string string; 

	string = "class :";

	node.m_symbolEntry = new SymbolTableEntry(string, Kind::_class, node.m_symbolTable);
	
	node.m_symbolTable->appendEntry(*node.getChildren()[0]->m_symbolEntry);


}

void SymbolTableCreatorVistor::visit(funcDef_Node& node)
{
	std::cout << "Visited funcDef_Node" << std::endl;

	std::string string;

	string = "function :";

	node.m_symbolEntry = new SymbolTableEntry(string, Kind::_function, node.m_symbolTable);
}

void SymbolTableCreatorVistor::visit(impleDef_Node& node)
{

}

void SymbolTableCreatorVistor::visit(inheritList_Node& node)
{
	std::cout << "Visited inheritList_Node" << std::endl;

	std::string string;

	for (auto i : node.getChildren())
	{
		node.m_symbolEntry = new SymbolTableEntry("id", Kind::_class, node.m_symbolTable);
	}

	
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

	std::string string = "id";
	
	node.m_symbolEntry = new SymbolTableEntry(string, Kind::_variable);
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

void SymbolTableCreatorVistor::print()
{
	

}

