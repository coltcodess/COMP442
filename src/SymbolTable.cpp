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

SymbolTableEntry* SymbolTable::getEntryByKind(Kind kind)
{
    for (SymbolTableEntry* entry : this->entries)
    {
        if (entry->kind == kind)
        {
            return entry;
        }
    }

    return nullptr;
}

bool SymbolTable::checkEntryInTable(SymbolTableEntry* entry)
{
    for (auto i : this->entries)
    {
        if (i->name.compare(entry->name) == 0 && i->kind == entry->kind && i->type.compare(entry->type) == 0) return true;
    }

    return false;
}

bool SymbolTable::checkEntryNameKindInTable(SymbolTableEntry* entry)
{
    for (auto i : this->entries)
    {
        if (i->name.compare(entry->name) == 0 && i->kind == entry->kind) return true;
    }

    return false;
}


