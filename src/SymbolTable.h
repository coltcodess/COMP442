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

		addSize();
	};

	SymbolTableEntry(std::string name, Kind kind, std::string type)
	{
		this->name = name;
		this->kind = kind;
		this->type = type;
		this->link = nullptr;

		addSize();

	};

	std::string name;
	Kind kind;
	std::string type;
	SymbolTable* link;

	// Assignment 5
	int m_entryOffset = 0;
	int m_entrySize = 0;

	void addSize()
	{
		if (this->type == "float")
		{
			this->m_entrySize = 8;
		}

		if (this->type == "int")
		{
			this->m_entrySize = 4;
		}
	}
};

class SymbolTable
{
public:

	SymbolTable(std::string name);
	SymbolTable();
	
	SymbolTable* parentTable;

	std::string getName();
	void setName(std::string name);
	const std::vector<SymbolTableEntry*> getEntries();
	void appendEntry(SymbolTableEntry* entry);
	void addAtFront(SymbolTableEntry* entry);
	SymbolTableEntry* getEntryByNameKind(std::string name, Kind kind);
	SymbolTableEntry* getEntryByKind(Kind kind);
	bool checkEntryInTable(SymbolTableEntry* entry);

	bool checkEntryNameKindInTable(SymbolTableEntry* entry);

	int m_tableOffset = 0;

private:
	std::string name;
	std::vector<SymbolTableEntry*> entries;

};



