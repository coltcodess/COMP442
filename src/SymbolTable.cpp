#include "SymbolTable.h"

SymbolTable::SymbolTable()
{

}

SymbolTable::SymbolTable(std::string name)
{
    this->name = name;
}

std::string SymbolTable::getName()
{
    return this->name;
}

void SymbolTable::setName(std::string name)
{
    this->name = name;
}

const std::vector<SymbolTableEntry*> SymbolTable::getEntries()
{
    return entries;
}

void SymbolTable::appendEntry(SymbolTableEntry* entry)
{

    entries.push_back(entry);
}

void SymbolTable::addAtFront(SymbolTableEntry* entry)
{
    std::vector<SymbolTableEntry*>::iterator it;
    it = entries.begin();
    it = entries.insert(it, 0, entry);
}

SymbolTableEntry* SymbolTable::getEntryByNameKind(std::string name, Kind kind)
{
    for (SymbolTableEntry* entry : this->entries)
    {
        if (entry->name == name && entry->kind == kind)
        {
            return entry;
        }
    }

    return nullptr;
}


