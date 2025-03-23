#include "SymbolTableCreatorVistor.h"

SymbolTableCreatorVistor::SymbolTableCreatorVistor(std::ofstream* output, std::ofstream* errors)
{
	m_output = output;
	m_errors = errors;
	m_global_table = new SymbolTable("global");
}

void SymbolTableCreatorVistor::visit(Node& node)
{

}

//////////////////
// Non-Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(prog_Node& node)
{
	node.m_symbolTable = m_global_table;

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

}

void SymbolTableCreatorVistor::visit(classDecl_Node& node)
{
	node.m_symbolTable = new SymbolTable(node.token->lexem);

	std::string name = node.token->lexem;

	node.m_symbolTable->parentTable = m_global_table;

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
	Node* prog = node.parent->parent;

	Node* classList = prog->getChild(Type::classDeclList);

	std::string className = node.token->lexem;

	SymbolTable* classTable = nullptr;
	SymbolTable* funcTable = nullptr;

	for (auto i : classList->getChildren())
	{
		if (i->token->lexem.compare(className) == 0 && !i->m_symbolTable->getEntries().empty())
		{
			classTable = i->m_symbolTable;

			for (auto j : classTable->getEntries())
			{
				if (j->name.compare(node.getChild(Type::funcDef)->token->lexem) == 0)
				{
					funcTable = j->link;
				}
			}
		}
	}

	if (funcTable != nullptr)
	{
		for (auto i : node.getChild(Type::funcDef)->m_symbolTable->getEntries())
		{
			funcTable->appendEntry(i);
		}
	}

}

void SymbolTableCreatorVistor::visit(inheritList_Node& node)
{
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
	std::string name = node.getChild(Type::idLit)->token->lexem;
	std::string type = node.getChild(Type::type)->token->convertTokenTypeToString();

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_variable, type, node.m_symbolTable);

}

//////////////////
// Terminals 
//////////////////

void SymbolTableCreatorVistor::visit(idLit_Node& node)
{
	std::string string = "id";
	
	node.m_symbolEntry = new SymbolTableEntry(string, Kind::_variable);
}

void SymbolTableCreatorVistor::visit(intLit_Node& node)
{
	


}

void SymbolTableCreatorVistor::visit(floatLit_Node& node)
{
	
}

void SymbolTableCreatorVistor::visit(type_Node& node)
{
	
}


void SymbolTableCreatorVistor::print()
{
	*m_output << " || table: " + m_global_table->getName() << std::endl;

	*m_output << " -------------------------------- " << std::endl;

	for (auto i : m_global_table->getEntries())
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
							if (k->kind == Kind::_parameter)
							{
								*m_output << "        param: | " + k->type + " | " + k->name << std::endl;
							}
							else if (k->kind == Kind::_variable)
							{
								*m_output << "        local: | " + k->type + " | " + k->name << std::endl;
							}
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

				for (auto j : i->link->getEntries())
				{
					*m_output << "        local: | " + j->type + " | " + j->name << std::endl;
				}

				*m_output << " -------------------------------- " << std::endl;
			}
		}

	}

}

