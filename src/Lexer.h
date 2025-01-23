#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

const int MAX_TOKEN_LENGTH = 512;

enum class TokenType
{
    // Punc 

    SEMI = ';',
    COLON = ':',
    CLOSEPAR = '(',
    OPENPAR = ')',
    OPENCUBR = '[',
    CLOSECUBR = ']',
    DOT = '.',
    COMMA = ',',

    // Operators
    MULTI = '*',
    PLUS = '+',
    MINUS = '-',
    DIV = '/',
    ARROW,
    OPENSQBR = '{',
    CLOSESQBR = '}',

    NOTEQ = 256,
    ASSIGN = 257,
    LEQ = 258,
    GEQ = 259,
    GT = 260,
    LT = 261,

    // Keywords 
    INT = 262,
    FLOAT = 263,
    LOCAL = 264,
    ATTRIBUTE = 265,
    THEN = 266,
    CLASS = 267,
    SELF = 268,
    PUBLIC = 269,
    PRIVATE = 270,
    IMPLEMENTATION = 271,
    RETURN = 272,
    WRITE = 273,
    READ = 274,
    IF = 275, 
    ELSE = 276,
    VOID = 277,
    CONSTRUCTOR = 278,
    WHILE = 279,   
    FUNCTION = 280,
    ISA = 281,
    EQ = 282,

    // Bitwise
    AND = 283,
    OR = 284,
    NOT = 285,

    // Data
    intnum = 286,
    floatnum = 287,

    // Comments
    inlinecmt = 288,
    blockcmt = 289,

    // Identifers
    id = 499,

    // Errors 
    ERROR = 500,

    // Invalid
    invalidchar = 501,
    invalidid = 502,
    invalidnum = 503
};

struct Token
{
    TokenType type = TokenType::ERROR; 
    std::string lexem = "ERROR";
    int position = 1;

    Token(TokenType t, std::string v, int p) : type(t), lexem(v), position(p) {};

    // Helper function for printing 
    std::string convertTokenTypeToString()
    {
        // Print out the token type as string for driver / debugging
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
        case TokenType::ISA:
            return "isa";
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
        case TokenType::NOT:
            return "not";
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
        case TokenType::invalidchar:
            return "invalidchar";

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

    // Backtracking one character within the SRC file
    char peekBackupChar();

    // Tokenize SRC file 
    void tokenize();
    Token* createToken(TokenType type, std::string value, int position);

    void initKeywords();

    // Checks within SRC file
    bool isDigit(char chr);
    bool isWhiteSpace(char chr);
    bool isNonzero(char chr);
    bool isAlpha(char chr);
    bool isOperator(char chr);
    bool isAlphaNumeric(char chr);
    bool isPunctuation(char chr);
    bool isInvalidChar(char chr);

    // Helper functions.
    std::string getNextWord();
    std::string getNextNumber();
    std::string getNextLine();
    TokenType getTokenType(std::string str);
    char peekNextChar();

    // SRC text from driver
    std::string m_sourceText;
    
    // Position within the SRC text
    int m_current_line_index = 0;

    // Line position within the SRC file 
    int m_current_line_number = 1;

    // Track error report lines
    int m_error_report_line_number = 1;

    // Get the position of the next token in the vector 
    int m_tokenIndex = 0;


    std::vector<Token*> m_tokens;
    std::unordered_map<std::string, TokenType> m_keywords;

};

#endif // LEXER_H