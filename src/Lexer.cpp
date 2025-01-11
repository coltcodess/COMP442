//
// Created by Colton LeBlond on 2024-12-19.
//

#include "Lexer.h"
#include <string.h>

// Constructor
Lexer::Lexer(const std::string source) : m_position(0), m_linePosition(0)
{
    std::cout << "Create Lexer" << std::endl;
    this->m_sourceText = source;
    this->tokenize();
}

bool Lexer::isIdentifier(char* chr)
{
    return false;
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

bool Lexer::isWhiteSpace(char chr)
{
    return chr == ' ' || chr == '\n' || chr == '\r';
}

bool Lexer::isAlpha(char chr)
{
    return (chr >= 'a' && chr <= 'z' || (chr >= 'A' && chr <= 'Z'));
}

bool Lexer::isAlphaNumeric(char chr)
{
    return isAlpha(chr) || isDigit(chr);
}

std::string Lexer::getNextWord()
{
    size_t start = m_position;
    

    return std::string();
}

std::string Lexer::getNextLine()
{  
    std::string temp = m_sourceText;
   
    std::string buffer;

    int buffer_index = 0;

    for (int i = m_position; i < temp.length(); i++)
    {
        if (temp[i] != '\n')
        {
            buffer.insert(buffer_index, 1, temp[i]);
            buffer_index++;
        }
        else
        {
            m_position = i + 1;
            return buffer;
        }
    }    

    return buffer;
}

bool Lexer::isOperator(char chr)
{
    return (chr == '+' || chr == '-' || chr == '/' || chr == '*');
}

bool Lexer::isNonzero(char chr)
{
    return (chr >= 1 || chr <= 9);
}



std::vector<Token>* Lexer::tokenize()
{
    std::vector<Token>* tokens = new std::vector<Token>; 

    while (m_position < m_sourceText.length())
    {
        char currentChar = m_sourceText[m_position];

        if (isWhiteSpace(currentChar))
        {
            m_position++;
            continue;
        }

        else if (isAlpha(currentChar))
        {

        }

        else if (isOperator(currentChar))
        {
            createToken(TokenType::OPERATOR, std::string{currentChar}, m_linePosition);
        }
    }

    return tokens;
}

Token Lexer::createToken(TokenType type, std::string value, int position)
{
    return Token(type, value, position);
}