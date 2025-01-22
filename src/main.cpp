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

    int file_position = 1;

    while (!lexer->isFinished())
    {
        
        // Handle user input to print next token
        //std::string input;
        //std::cout << "Press Y/N to get the next token. " << std::endl;
        //std::cin >> input;

        // Log valid token
        Token* token = lexer->getNextToken();

        if (token->position > file_position)
        {
            tokenOutputFile << '\n';
            file_position++;
        }

        std::string output = "[" + token->convertTokenTypeToString() + ", " + token->lexem + ", " + std::to_string(token->position) + "]";
        std::cout << output << std::endl;
        tokenOutputFile << output;

    }

    std::cout << "Finished lexical analysis" << std::endl;

    tokenOutputFile.close();
    errorOutputFile.close();

    return 0;
}
