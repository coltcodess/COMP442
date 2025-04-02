#include "SymbolTableCreatorVistor.h"

SymbolTableCreatorVistor::SymbolTableCreatorVistor(std::ofstream* output, std::ofstream* errors)
{
	m_output = output;
	m_errors = errors;
	m_global_table = new SymbolTable("global");
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
	node.m_symbolTable = m_global_table;
	bool mainIncluded = false;

	// ClassDeclList
	for (Node* i : node.getChildren()[0]->getChildren())
	{
		// Semantic check - Multiple Classes 
		if (node.m_symbolTable->checkEntryInTable(i->m_symbolEntry))
		{
			*m_errors << "ERROR: global - Multiply declared class - " + i->m_symbolEntry->name << std::endl;
			continue;
		}

		node.m_symbolTable->appendEntry(i->m_symbolEntry);
	}

	//FuncDeclList
	for (auto i : node.getChildren()[2]->getChildren())
	{
		if (i->token->lexem.compare("main") == 0)
		{
			if (!mainIncluded)
			{
				node.m_symbolTable->appendEntry(i->m_symbolEntry);
				mainIncluded = true;
			}
			else
			{
				*m_errors << "ERROR:  - Too manay main() functions found " << std::endl;
			}
		}
	}

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
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
					// Semantic Check - Same Member variable added
					if (node.m_symbolTable->checkEntryNameKindInTable(j->m_symbolEntry))
					{
						*m_errors << "ERROR: " + node.token->lexem + " - Multiply declared data member - " + j->m_symbolEntry->name << std::endl;
					}
					else
					{
						node.m_symbolTable->appendEntry(j->m_symbolEntry);
					}
				}


			}
			else if (i->getType() == Type::memDeclFunc)
			{
				// Semantic Check - Same Member functions added
				if (node.m_symbolTable->checkEntryNameKindInTable(i->m_symbolEntry))
				{
					if (node.m_symbolTable->checkEntryInTable(i->m_symbolEntry))
					{
						*m_errors << "ERROR: " + node.token->lexem + " - Multiply declared function with same parameters - " + i->m_symbolEntry->name << std::endl;
						continue;
					}

					*m_errors << "WARNING: " + node.token->lexem + " - Multiply declared function with same parameters - " + i->m_symbolEntry->name << std::endl;
				}


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

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

}

void SymbolTableCreatorVistor::visit(funcDef_Node& node)
{
	node.m_symbolTable = new SymbolTable(node.token->lexem);

	std::string name = node.token->lexem;
	std::string type = node.token->convertTokenTypeToString();

	for (auto i : node.getChildren())
	{
		if (i->getType() == Type::statBlock)
		{
			for (auto k : i->getChildren())
			{
				if (k->getType() == Type::varDecl)
				{
					node.m_symbolTable->appendEntry(k->m_symbolEntry);
				}
			}
		}
	}

	for (auto i : node.getChildren())
	{
		if (i->getType() == Type::fParamsList)
		{
			for (auto j : i->getChildren())
			{
				node.m_symbolTable->appendEntry(j->m_symbolEntry);
			}
		}
	}

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_function, node.m_symbolTable);
	
}

void SymbolTableCreatorVistor::visit(impleDef_Node& node)
{
	Node* prog = node.parent->parent;
	Node* classList = prog->getChild(Type::classDeclList);


	SymbolTable* classTable = nullptr;
	std::string className = node.token->lexem;
	std::vector<Node*> classDecl = classList->getChildren();
	
	bool foundClassDef = false;

	// Semantic check - Undefined Class for definition
	for (auto i : classDecl)
	{
		if (i->token->lexem == node.token->lexem) foundClassDef = true;
	}
	if (!foundClassDef)
	{
		*m_errors << "ERROR: Definition provided for undeclared member function - " + node.token->lexem << std::endl;
		return;
	}

	// Get class table 
	for (auto i : classList->getChildren())
	{
		if (i->token->lexem.compare(className) == 0)
		{
			classTable = i->m_symbolTable;
		}
	}

	// All implement functions
	for (auto i : node.getChildren())
	{
		if (i->getType() == Type::funcDef)
		{
			//Semantic Check
			if (classTable->getEntries().empty())
			{
				*m_errors << "ERROR: Definition provided for undeclared member function - " + node.token->lexem << std::endl;
				return;
			}

			for (auto j : classTable->getEntries())
			{
				if (i->token->lexem == j->name)
				{
					for (auto k : i->m_symbolTable->getEntries())
					{
						j->link->appendEntry(k);
					}
				}
			}
		}

	}

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
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
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(memDeclFunc_Node& node)
{

	node.m_symbolTable = new SymbolTable(node.token->lexem);
	node.m_symbolTable->parentTable = node.parent->parent->m_symbolTable;

	std::string name = node.token->lexem;
	std::string type;

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_function, type, node.m_symbolTable);

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

	std::string name = node.getChild(Type::idLit)->token->lexem;
	std::string type = node.getChild(Type::type)->token->convertTokenTypeToString();
	
	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_parameter, type);

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(varDecl_Node& node)
{
	std::string name = node.token->lexem;
	std::string type = node.token->convertTokenTypeToString();

	node.m_symbolEntry = new SymbolTableEntry(name, Kind::_variable, type, node.m_symbolTable);

	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(assignOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->m_symbolTable = node.m_symbolTable;
		child->accept(*this);
	}
}

void SymbolTableCreatorVistor::visit(multiOp_Node& node)
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
	std::string string = "id";
	
	node.m_symbolEntry = new SymbolTableEntry(string, Kind::_variable);

}

void SymbolTableCreatorVistor::visit(intLit_Node& node)
{
	node.m_symbolEntry = new SymbolTableEntry(node.token->lexem);
	node.m_symbolEntry->type = Type::intLit;

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
}

void SymbolTableCreatorVistor::print()
{
	*m_output << " || table: " + m_global_table->getName() + "  offset:  " + std::to_string(m_global_table->m_tableOffset) << std::endl;

	*m_output << " -------------------------------- " << std::endl;

	for (auto i : m_global_table->getEntries())
	{

		if (i != nullptr)
		{
			// Classes
			if (i->kind == Kind::_class)
			{
				*m_output << "  || class: " + i->name + " scope offset:  " + std::to_string(i->link->m_tableOffset) << std::endl;

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
								*m_output << "        param: | " + k->type + " | " + k->name + " | " + std::to_string(k->m_entrySize) + " | " + std::to_string(k->m_entryOffset) << std::endl;
							}
							else if (k->kind == Kind::_variable)
							{
								*m_output << "        local: | " + k->type + " | " + k->name + " | " + std::to_string(k->m_entrySize) + " | " + std::to_string(k->m_entryOffset) << std::endl;
							}
						}

					}
					else if (j->kind == Kind::_variable)
					{
						*m_output << "  data:  | " + j->name + " | " + j->type + " | " + std::to_string(j->m_entrySize) + " | " + std::to_string(j->m_entryOffset) << std::endl;
					}

				}

				*m_output << " -------------------------------- " << std::endl;
			}

			// Free functions
			else if (i->kind == Kind::_function)
			{

				*m_output << "  || function: " + i->name + "  offset:  " + std::to_string(i->m_entryOffset) << std::endl;

				for (auto j : i->link->getEntries())
				{
					if (j->kind == Kind::_variable)
					{
						*m_output << "        local: | " + j->type + " | " + j->name + " | " + std::to_string(j->m_entrySize) << std::endl;
					}	
					else if (j->kind == Kind::_parameter)
					{
						*m_output << "        param: | " + j->type + " | " + j->name << std::endl;
					}
				}

				*m_output << " -------------------------------- " << std::endl;
			}
		}

	}

}



