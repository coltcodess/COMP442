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
    int position = 1;

    Token(TokenType t, std::string v, int p) : type(t), lexem(v), position(p) {};

    // Helper function for printing 
    std::string convertTokenTypeToString()
    {

        // Print out the token type as string
        switch (type) {
        case TokenType::ATTRIBUTE:
            return "attribute";
        case TokenType::INT:
            return "int";
        case TokenType::WHILE:
            return "while";
        case TokenType::PLUS:
            return "plus";
        case TokenType::intnum:
            return "intnum";
        case TokenType::ARROW:
            return "arrow";
        case TokenType::AND:
            return "and";
        case TokenType::ASSIGN:
            return "assign";
        case TokenType::blockcmt:
            return "blockcmt";
        case TokenType::CLASS:
            return "class";
        case TokenType::CLOSECUBR:
            return "closecubr";
        case TokenType::CLOSEPAR:
            return "closepar";
        case TokenType::CLOSESQBR:
            return "closesqbr";
        case TokenType::COLON:
            return "colon";
        case TokenType::COMMA:
            return "comma";
        case TokenType::CONSTRUCTOR:
            return "constructor";
        case TokenType::DIV:
            return "div";
        case TokenType::DOT:
            return "dot";
        case TokenType::ELSE:
            return "else";
        case TokenType::EQ:
            return "eq";
        case TokenType::FLOAT:
            return "float";
        case TokenType::floatnum:
            return "floatnum";
        case TokenType::FUNCTION:
            return "function";
        case TokenType::GEQ:
            return "geq";
        case TokenType::GT:
            return "gt";
        case TokenType::IF:
            return "if";
        case TokenType::IMPLEMENTATION:
            return "Implementation";
        case TokenType::inlinecmt:
            return "inlinecmt";
        case TokenType::ISA:
            return "isa";
        case TokenType::LEQ:
            return "leq";
        case TokenType::LOCAL:
            return "local";
        case TokenType::LT:
            return "lt";
        case TokenType::MINUS:
            return "minus";
        case TokenType::MULTI:
            return "multi";
        case TokenType::NOTEQ:
            return "noteq";
        case TokenType::OPENCUBR:
            return "opencubr";
        case TokenType::OPENPAR:
            return "openpar";
        case TokenType::OPENSQBR:
            return "opensqbr";
        case TokenType::OR:
            return "or";
        case TokenType::PRIVATE:
            return "private";
        case TokenType::PUBLIC:
            return "public";
        case TokenType::READ:
            return "read";
        case TokenType::RETURN:
            return "return";
        case TokenType::SELF:
            return "self";
        case TokenType::SEMI:
            return "semi";
        case TokenType::THEN:
            return "then";
        case TokenType::VOID:
            return "void";
        case TokenType::WRITE:
            return "write";
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::id:
            return "id";

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
    std::string getNextWord();
    std::string getNextNumber();
    std::string getNextLine();
    TokenType getTokenType(std::string str);

    // SRC text from driver
    std::string m_sourceText;
    
    // Position within the SRC text
    int m_position = 0;

    // Line position within the SRC file 
    int m_linePosition = 1;

    // Get the position of the next token in the vector 
    int m_tokenIndex = 0;


    std::vector<Token*> m_tokens;
    std::unordered_map<std::string, TokenType> m_keywords;

};

#endif // LEXER_H