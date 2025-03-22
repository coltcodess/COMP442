#pragma once

#include <iostream>
#include <vector>
#include "Kind.h"

class SymbolTable;

struct SymbolTableEntry
{
public:

	SymbolTableEntry(std::string name, Kind kind, SymbolTable* symbolTable)
	{
		this->name = name;
		this->kind = kind;
		this->link = symbolTable;
	};

	SymbolTableEntry(std::string name, Kind kind)
	{
		this->name = name;
		this->kind = kind;
		this->link = nullptr;
	};

	SymbolTableEntry(std::string name, Kind kind, std::string type, SymbolTable* symbolTable)
	{
		this->name = name;
		this->kind = kind;
		this->type = type;
		this->link = symbolTable;
	};

	SymbolTableEntry(std::string name, Kind kind, std::string type)
	{
		this->name = name;
		this->kind = kind;
		this->type = type;
		this->link = nullptr;
	};

	std::string name;
	Kind kind;
	std::string type;
	SymbolTable* link;

};

class SymbolTable
{
public:

	SymbolTable(std::string name);
	SymbolTable();
	
	SymbolTable* parentTable;

	std::string getName();
	void setName(std::string name);
	const std::vector<SymbolTableEntry>& getEntries();
	void appendEntry(SymbolTableEntry entry);
	void addAtFront(SymbolTableEntry entry);

private:
	std::string name;
	std::vector<SymbolTableEntry> entries;

};



