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
    Lexer(const std::ifstream* file);

    bool isIdentifier(char* chr);
    bool isKeyword(const std::string chr);
    bool isDigit(char chr);
    bool isWhiteSpace(char chr);
    bool isAlphaNumeric(char chr);
    bool isComment(char* chr);

    void tokenize();
    std::string getNextWord();
    std::string getNextNumber();

private:
    const std::ifstream* m_inputFile;
};

#endif // LEXER_H