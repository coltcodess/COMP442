#include "Lexer.h"
//
// Created by Colton LeBlond on 2024-12-19.
//

#include "Lexer.h"

#include <string.h>

// Constructor
Lexer::Lexer(const std::ifstream* sourcefile)
{
    std::cout << "Create Lexer" << std::endl;
    this->m_inputFile = sourcefile;
}

bool Lexer::isKeyword(const std::string chr)
{

    const std::string keywords[] = { "int", "float", "if", "while" };

    for (auto& keyword : keywords)
    {
        if (keywords->compare(chr) == 0)
        {
            return true;
        }
    }

    return false;
}

bool Lexer::isDigit(char chr)
{
    return (chr >= '0' && chr <= '9');
}

void Lexer::tokenize()
{

    return;
}