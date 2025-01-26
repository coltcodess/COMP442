#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"

const std::string SOURCE_FILE_TYPE = ".src";
const std::string OUTPUT_TOKEN_FILE_TYPE = ".outlextokens";

int main()
{
    // Intro message:
    std::string fileInput;
    std::cout << "Enter file to open. " << std::endl;
    std::cin >> fileInput;

    std::stringstream* buffer = new std::stringstream;
    std::ifstream* srcFile = NULL;
    srcFile = new std::ifstream(fileInput + SOURCE_FILE_TYPE);
    bool file_opened = false;

    // Reask for valid file
    if (srcFile->fail())
    {
        while (!file_opened)
        {
            std::cout << "Opps file not found: Enter file to open. " << std::endl;
            std::cin >> fileInput;

            srcFile = new std::ifstream(fileInput + SOURCE_FILE_TYPE);

            if (srcFile->good())
            {
                file_opened = true;
            }
            else
            {
                file_opened = false;
            }
        }
    }
    
    // Read source into string buffer
    if (srcFile != NULL)
    {
        *buffer << srcFile->rdbuf();
    }

    srcFile->close();
    srcFile = NULL;

    // Create Lexer with source file  
    Lexer* lexer = new Lexer(buffer->str(), fileInput);

    // Write token / error files (MOVE to Lexer - TODO)
    std::ofstream tokenOutputFile(fileInput + OUTPUT_TOKEN_FILE_TYPE, std::ofstream::out);

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

        std::string output = "[" + token->convertTokenTypeToString() + ", " + token->lexem + ", " + std::to_string(token->position) + "] ";
        std::cout << output << std::endl;
        tokenOutputFile << output;

    }

    std::cout << "Finished lexical analysis...." << std::endl;

    tokenOutputFile.close();
   

    return 0;
}
