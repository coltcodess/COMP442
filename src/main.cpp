#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"

int main()
{
    std::string fileInput;
    std::cout << "Enter file to open. " << std::endl;
    std::cin >> fileInput;

    std::stringstream* buffer = new std::stringstream;
    std::ifstream srcFile(fileInput + ".src");

    if (srcFile.is_open())
    {
        std::cout << "DEBUG - Opened file" << std::endl;
    }
    else
    {
        std::cout << "ERROR - Failed to Open file" << std::endl;
        return 1;
    }

    *buffer << srcFile.rdbuf();

    srcFile.close();

    // Create Lexer with source file  
    Lexer* lexer = new Lexer(buffer->str());

    // Write token / error files  
    std::ofstream tokenOutputFile(fileInput + ".outlextokens", std::ofstream::out);
    std::ofstream errorOutputFile(fileInput + ".outlexerrors", std::ofstream::out);

    int file_position = 0;

    while (!lexer->isFinished())
    {
        // Log valid token
        Token* token = lexer->getNextToken();
        std::string output = "[" + token->convertTokenTypeToString() + ", " + token->lexem + ", " + std::to_string(token->position) + "]" + '\n';
        tokenOutputFile << output;

        // Log error token 
    }

    tokenOutputFile.close();
    errorOutputFile.close();

    return 0;
}
