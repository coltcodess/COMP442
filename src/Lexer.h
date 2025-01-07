//
// Created by Colton LeBlond on 2024-12-19.
//

#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <fstream>
#include <unordered_map>

enum class TokenType
{
    KEYWORD, 
    IDENIFIER,
    OPERATOR, 
};

struct Token
{
    TokenType type; 
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {};
};

class Lexer
{

public:
    Lexer(const std::string source);

    bool isIdentifier(char* chr);
    bool isKeyword(const std::string chr);
    bool isDigit(char chr);
    bool isWhiteSpace(char chr);
    bool isAlpha(char chr);
    bool isAlphaNumeric(char chr);
    bool isComment(char* chr);

    std::vector<Token> tokenize();
    std::string getNextWord();
    std::string getNextNumber();
    std::string getNextLine();

private:
    std::string m_sourceText;
    std::string m_currentLine;
    size_t m_position;
    size_t m_linePosition;
};

#endif // LEXER_H