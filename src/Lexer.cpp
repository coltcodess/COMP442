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

//bool Lexer::isKeyword(const std::string chr)
//{
//    const std::string keywords[] = { "int", "float", "if", "while" };
//
//    for (auto& keyword : keywords)
//    {
//        if (keywords->compare(chr) == 0)
//        {
//            return true;
//        }
//    }
//
//    return false;
//}

bool Lexer::isDigit(char chr)
{
    return (chr >= '0' && chr <= '9');
}

bool Lexer::isNonzero(char chr)
{
    return (chr >= 1 || chr <= 9);
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
    return isAlpha(chr) || isDigit(chr) || chr == '_';
}

bool Lexer::isOperator(char chr)
{
    return (chr == '+' || chr == '-' || chr == '/' || chr == '*');
}

std::string Lexer::getNextWord()
{
    size_t start = m_position;
    while (m_position < m_sourceText.length() && isAlphaNumeric(m_sourceText[m_position]))
    {
        m_position++;
    }
    return m_sourceText.substr(start, m_position);
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

std::string Lexer::getNextChar()
{
    return std::string();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens; 

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
            std::string word = getNextWord();

            // Look through map for word
            if (m_keywords.find(word) != m_keywords.end())
            {
                // Add keyword
            }
            else
            {
                // Add Identifier
                
            }
        }

        else if (isOperator(currentChar))
        {
            //createToken(TokenType::OPERATOR, std::string{currentChar}, m_linePosition);
            m_position++;
        }

        if (currentChar == '\n')
        {
            m_linePosition++;
            m_position++;
        }


    }

    return tokens;
}

Token* Lexer::createToken(TokenType type, std::string value, int position)
{
    Token* token = new Token(type, value, position);
    return token;
}

std::string Lexer::convertTokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::KEYWORD:
        return "KEYOWRD";
    case TokenType::IDENIFIER:
        return "IDENIFIER";
    case TokenType::OPERATOR:
        return "OPERATOR";
    }
}

void Lexer::initKeywords()
{
    // Keywords
    m_keywords["int"] = TokenType::KEYWORD;
    m_keywords["float"] = TokenType::KEYWORD;
    m_keywords["void"] = TokenType::KEYWORD;
    m_keywords["class"] = TokenType::KEYWORD;
    m_keywords["self"] = TokenType::KEYWORD;
    m_keywords["isa"] = TokenType::KEYWORD;
    m_keywords["implementation"] = TokenType::KEYWORD;
    m_keywords["while"] = TokenType::KEYWORD;
    m_keywords["if"] = TokenType::KEYWORD;
    m_keywords["then"] = TokenType::KEYWORD;
    m_keywords["else"] = TokenType::KEYWORD;
    m_keywords["read"] = TokenType::KEYWORD;
    m_keywords["write"] = TokenType::KEYWORD;
    m_keywords["return"] = TokenType::KEYWORD;
    m_keywords["local"] = TokenType::KEYWORD;
    m_keywords["constructor"] = TokenType::KEYWORD;
    m_keywords["attribute"] = TokenType::KEYWORD;
    m_keywords["function"] = TokenType::KEYWORD;
    m_keywords["public"] = TokenType::KEYWORD;
    m_keywords["private"] = TokenType::KEYWORD;

    // Operators (move out if needed)
    m_keywords["or"] = TokenType::OPERATOR;
    m_keywords["and"] = TokenType::OPERATOR;
    m_keywords["not"] = TokenType::OPERATOR;

}
