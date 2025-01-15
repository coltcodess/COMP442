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
    m_tokens = this->tokenize();

    // remove
    m_tokens.push_back(Token(TokenType::ATTRIBUTE, "wow", 1));
       
}

Token Lexer::getNextToken()
{
    Token token = m_tokens[m_tokenIndex];
    return token;
}

bool Lexer::isIdentifier(char* chr)
{
    return false;
}

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

char Lexer::getNextChar()
{
    if (m_position != m_sourceText.length())
    {
        char currentChar = m_sourceText[m_position];
        m_position++;
        return currentChar;
    }
    else
    {
        std::cout << "End of source file";
        return ' ';
    }

}

char Lexer::backupChar()
{
    if (m_position != 0)
    {
        int temp_pos = m_position;
        temp_pos--;
        char backChar = m_sourceText[temp_pos];
        return backChar;
    }
    else return ' ';
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

        // REMOVE ______________________________

        m_position++;


    }

    return tokens;
}

Token* Lexer::createToken(TokenType type, std::string value, int position)
{
    Token* token = new Token(type, value, position);
    return token;
}

void Lexer::initKeywords()
{
    // Keywords
    m_keywords["int"] = TokenType::INT;
    m_keywords["float"] = TokenType::FLOAT;
    m_keywords["void"] = TokenType::VOID;
    m_keywords["class"] = TokenType::CLASS;
    m_keywords["self"] = TokenType::SELF;
    m_keywords["isa"] = TokenType::ISA;
    m_keywords["implementation"] = TokenType::IMPLEMENTATION;
    m_keywords["while"] = TokenType::WHILE;
    m_keywords["if"] = TokenType::IF;
    m_keywords["then"] = TokenType::THEN;
    m_keywords["else"] = TokenType::ELSE;
    m_keywords["read"] = TokenType::READ;
    m_keywords["write"] = TokenType::WRITE;
    m_keywords["return"] = TokenType::RETURN;
    m_keywords["local"] = TokenType::LOCAL;
    m_keywords["constructor"] = TokenType::CONSTRUCTOR;
    m_keywords["attribute"] = TokenType::ATTRIBUTE;
    m_keywords["function"] = TokenType::FUNCTION;
    m_keywords["public"] = TokenType::PUBLIC;
    m_keywords["private"] = TokenType::PRIVATE;

}
