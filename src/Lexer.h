#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "Token.h"

const int MAX_TOKEN_LENGTH = 512;

class Lexer
{

public:
    Lexer(const std::string source, const std::string fileName);

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
    std::string getNextBlock();
    TokenType getTokenType(std::string str);
    char peekNextChar();

    // Log function
    void logMessage(std::string s, TokenType tokenType);

    // SRC text from driver
    std::string m_sourceText;
    std::string m_sourceFileName;
    
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
    std::ofstream* m_errorOutputFile;
};

#endif // LEXER_H