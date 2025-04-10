#pragma once

#include <iostream>
#include <vector>
#include "Kind.h"
#include <string>

class SymbolTableEntry;

class SymbolTable
{
public:
	std::string name;
	SymbolTable(int tableLevel, std::string name, SymbolTable* upperTable);
	
	std::string getName();
	int tablelevel = 0;
	SymbolTable* upperTable;
	void setName(std::string name);
	const std::vector<SymbolTableEntry*> getEntries();
	void appendEntry(SymbolTableEntry* entry);
	void addAtFront(SymbolTableEntry* entry);
	SymbolTableEntry* getEntryByNameKind(std::string name, Kind kind);
	SymbolTableEntry* getEntryByKind(Kind kind);
	SymbolTableEntry* getEntryByName(std::string name);
	bool checkEntryInTable(SymbolTableEntry* entry);
	std::string print();

	bool checkEntryNameKindInTable(SymbolTableEntry* entry);

	SymbolTableEntry* lookupName(std::string name);

	int m_tableOffset = 0;


private:

	std::vector<SymbolTableEntry*> entries;

};

class SymbolTableEntry
{
public:

	SymbolTableEntry(std::string name)
	{
		this->name = name;
	};

	SymbolTableEntry(std::string name, Kind kind, SymbolTable* symbolTable)
	{
		this->name = name;
		this->kind = kind;
		this->link = symbolTable;
	};

	SymbolTableEntry(std::string name, SymbolTable* symbolTable)
	{
		this->name = name;
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

	SymbolTableEntry(std::string name, std::vector<SymbolTableEntry*>& entries, SymbolTable* symbolTable, std::string type)
	{
		this->name = name;
		this->link = symbolTable;
		this->entries = entries;
		this->type = type;

	};

	SymbolTableEntry(std::string name, Kind kind, std::string type)
	{
		this->name = name;
		this->kind = kind;
		this->type = type;
		this->link = nullptr;
	};

	SymbolTableEntry(std::string name, Kind kind, std::string type, std::vector<int> arraylist)
	{
		this->name = name;
		this->kind = kind;
		this->type = type;
		this->link = nullptr;
		this->arrayList = arraylist;
	};

	std::string entryToString()
	{
		std::string dimlist;
		for (int i : this->arrayList)
		{
			dimlist += "[" + std::to_string(i) + "]";
		}

		if (link != nullptr)
		{
			return " |  " + this->name + " | " + this->type + link->print();

		}
		else
		{
			return " |  " + this->name + " | " + this->type + dimlist + " | " + std::to_string(this->m_entrySize);
		}
	}



	std::string name;
	Kind kind;
	std::string type;
	SymbolTable* link = nullptr;
	std::vector<SymbolTableEntry*> entries;
	std::vector<int> arrayList;

	// Assignment 5
	int m_entryOffset = 0;
	int m_entrySize = 0;

};



