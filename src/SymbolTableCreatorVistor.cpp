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
	m_tables.push_back(*node.m_symbolTable);

	*m_output << " || table: " + node.m_symbolTable->getName() << std::endl;

	*m_output << " -------------------------------- " << std::endl;


	// ClassDeclList
	for (Node* i : node.getChildren()[0]->getChildren())
	{
		node.m_symbolTable->appendEntry(i->m_symbolEntry);
	}

	//FuncDeclList
	for (auto i : node.getChildren()[2]->getChildren())
	{
		node.m_symbolTable->appendEntry(i->m_symbolEntry);
	}

	// ImpleDecList
	for (auto i : node.getChildren()[1]->getChildren())
	{
		if (i->getType() == Type::funcDef)
		{

		}
	}

	for (auto i : node.m_symbolTable->getEntries())
	{
		// Print classes
		if (i != nullptr)
		{
			if (i->kind == Kind::_class)
			{
				*m_output << "  || class: " + i->name << std::endl;

				if (i->link->getEntryByKind(Kind::_inherit) != nullptr)
				{
					*m_output << "  inherit: | " + i->link->getEntryByKind(Kind::_inherit)->name << std::endl;
				}
				else
				{
					*m_output << "  inherit: | none" << std::endl;
				}

				for (auto j : i->link->getEntries())
				{

					if (j->kind == Kind::_function)
					{
						*m_output << " -------------------------------- " << std::endl;
						*m_output << "        table:  " + i->name + "::" + j->name << std::endl;
						*m_output << " -------------------------------- " << std::endl;

						for (auto k : j->link->getEntries())
						{
							*m_output << "        param: | " + k->type + " | " + k->name << std::endl;
						}

					}
					else if (j->kind == Kind::_variable)
					{
						*m_output << "  data:  | " + j->name + " | " + j->type << std::endl;
					}

				}

				*m_output << " -------------------------------- " << std::endl;
			}

			else if (i->kind == Kind::_function)
			{

				*m_output << "  || function: " + i->name << std::endl;
				*m_output << " -------------------------------- " << std::endl;
			}
		}
		
	}
}


void SymbolTableCreatorVistor::visit(classDecl_Node& node)
{
	std::cout << "Visited classDecl_Node" << std::endl;

	node.m_symbolTable = new SymbolTable(node.token->lexem);
	m_tables.push_back(*node.m_symbolTable);

	std::string name = node.token->lexem;

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_class, node.m_symbolTable);
	
	// Members 
	if (!node.getChildren()[2]->getChildren().empty())
	{
		for (auto i : node.getChildren()[2]->getChildren())
		{
			// Member data
			if (i->getType() == Type::memDeclAttrib)
			{
				for (auto j : i->getChildren())
				{
					node.m_symbolTable->appendEntry(j->m_symbolEntry);
				}
			}
			else if (i->getType() == Type::memDeclFunc)
			{
				node.m_symbolTable->appendEntry(i->m_symbolEntry);
				i->m_symbolTable->parentTable = node.m_symbolTable;
			}
		}
	}

	// Inheritance
	if (!node.getChildren()[1]->getChildren().empty())
	{
		node.m_symbolTable->appendEntry(node.getChildren()[1]->m_symbolEntry);
	}


}

void SymbolTableCreatorVistor::visit(funcDef_Node& node)
{
	std::cout << "Visited funcDef_Node" << std::endl;

	// Temp Table
	node.m_symbolTable = new SymbolTable();

	std::string name = node.token->lexem;
	std::string type = node.token->convertTokenTypeToString();

	for (auto i : node.getChildren())
	{
		if (i->getType() == Type::statBlock)
		{
			for (auto j : i->m_symbolTable->getEntries())
			{
				node.m_symbolTable->appendEntry(j);
			}
		}
	}

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_function, node.m_symbolTable);
	
}

void SymbolTableCreatorVistor::visit(impleDef_Node& node)
{
	std::cout << "Visited funcDef_Node" << std::endl;

	for (auto i : m_tables)
	{
		if (i.getName() == node.token->lexem)
		{
			for (auto j : node.getChildren())
			{
				if (j->getType() == Type::funcDef && !j->m_symbolTable->getEntries().empty())
				{
					for (auto k : j->m_symbolTable->getEntries())
					{
						i.appendEntry(k);
					}
				}
			}
		}
	}
}

void SymbolTableCreatorVistor::visit(inheritList_Node& node)
{
	std::cout << "Visited inheritList_Node" << std::endl;


	if (!node.getChildren().empty())
	{
		std::string name = node.getChild(Type::idLit)->token->lexem;

		node.m_symbolEntry = new SymbolTableEntry(name, Kind::_inherit, node.m_symbolTable);
	}

}

void SymbolTableCreatorVistor::visit(memDeclAttrib_Node& node)
{

}

void SymbolTableCreatorVistor::visit(memDeclFunc_Node& node)
{

	node.m_symbolTable = new SymbolTable(node.token->lexem);
	m_tables.push_back(*node.m_symbolTable);
	node.m_symbolTable->parentTable = node.parent->parent->m_symbolTable;

	std::string name = node.token->lexem;
	std::string type = node.getChild(Type::type)->token->convertTokenTypeToString();


	// fParams
	if (!node.getChildren()[2]->getChildren().empty())
	{
		for (auto i : node.getChildren()[2]->getChildren())
		{			
			node.m_symbolTable->appendEntry(i->m_symbolEntry);
		}
	}

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_function, type, node.m_symbolTable);
}

void SymbolTableCreatorVistor::visit(statBlock_Node& node)
{
	node.m_symbolTable = new SymbolTable();

	if (!node.getChildren().empty())
	{
		for (auto i : node.getChildren())
		{
			if (i->getType() == Type::varDecl)
			{
				node.m_symbolTable->appendEntry(i->m_symbolEntry);
			}

		}
	}

}

void SymbolTableCreatorVistor::visit(fParam_Node& node)
{

	std::string name = node.getChild(Type::idLit)->token->lexem;
	std::string type = node.getChild(Type::type)->token->convertTokenTypeToString();
	

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_parameter, type);
}

void SymbolTableCreatorVistor::visit(varDecl_Node& node)
{
	std::cout << "Visited varDecl_Node" << std::endl;
	
	std::string name = node.getChild(Type::idLit)->token->lexem;
	std::string type = node.getChild(Type::type)->token->convertTokenTypeToString();


	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_variable, type, node.m_symbolTable);

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


