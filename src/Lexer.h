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
    // Data
    intnum, 
    floatnum,
    id,
    inlinecmt,
    blockcmt, 

    // Punc 
    EQ,
    NOTEQ,
    SEMI,
    COLON,
    CLOSEPAR,
    OPENPAR,
    CLOSECUBR,
    OPENCUBR,
    DOT,
    COMMA,
    
    AND,
    OR,
    ISA,

    // Operators
    MULTI,
    PLUS,
    MINUS,
    DIV,
    ARROW,
    OPENSQBR,
    CLOSESQBR,
    ASSIGN,
    LEQ,
    GEQ,
    GT,
    LT,

    // Keywords 
    INT,
    FLOAT,
    LOCAL,
    ATTRIBUTE,
    THEN,
    CLASS,
    SELF,
    PUBLIC,
    PRIVATE,
    IMPLEMENTATION,
    RETURN,
    WRITE,
    READ,
    IF, 
    ELSE,
    VOID,
    CONSTRUCTOR,
    WHILE,   
    FUNCTION,

    // Errors 
    ERROR,
};

enum class State
{

};

struct Token
{
    TokenType type = TokenType::ATTRIBUTE; 
    std::string lexem = "ERROR";
    int position = 0;

    Token(TokenType t, std::string v, int p) : type(t), lexem(v), position(p) {};

    // Helper function for printing 
    std::string convertTokenTypeToString()
    {
        switch (type) {
        case TokenType::ATTRIBUTE:
            return "attribute";
        case TokenType::INT:
            return "int";

        default:
            return "$$$$$";
        }

    }
};

class Lexer
{

public:
    Lexer(const std::string source);

    // Return the next Token to be used. (Should this be pointer to the Token object??)
    Token* getNextToken();
    bool isFinished();

private:

    // Gets the next character within the SRC file
    char getNextChar();

    // (TO-DO) Checks whether the lexer is in a Final state??? 
    bool isFinalState(State state);

    // Backtracking one character within the SRC file
    char backupChar();

    // Tokenize SRC file 
    void tokenize();
    Token* createToken(TokenType type, std::string value, int position);

    void initKeywords();

    // Checks within SRC file
    bool isIdentifier(char* chr);
    bool isDigit(char chr);
    bool isWhiteSpace(char chr);
    bool isNonzero(char chr);
    bool isAlpha(char chr);
    bool isOperator(char chr);
    bool isAlphaNumeric(char chr);
    bool IsPunctuation(char chr);

    // Helper functions.
    void removeSourceWhiteSpace();
    std::string getNextWord();
    std::string getNextNumber();
    std::string getNextLine();

    // SRC text from driver
    std::string m_sourceText;
    
    // Position within the SRC text
    size_t m_position = 0;

    // Line position within the SRC file 
    size_t m_linePosition = 0;

    // Get the position of the next token in the vector 
    int m_tokenIndex = 0;


    std::vector<Token*> m_tokens;
    std::unordered_map<std::string, TokenType> m_keywords;

};

#endif // LEXER_H