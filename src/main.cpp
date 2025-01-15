#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"

static std::string _TEST_DIR = ".\\data\\";

int main()
{
    std::string fileInput;
    std::cout << "Enter file to open. " << std::endl;
    std::cin >> fileInput;

    std::stringstream* buffer = new std::stringstream;
    std::ifstream srcFile(_TEST_DIR + fileInput + ".src");

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


    Lexer* lexer = new Lexer(buffer->str());

    Token token = lexer->getNextToken();

    std::cout << "[" + token.convertTokenTypeToString() + ", " + token.lexem + ", " + std::to_string(token.position) + "]" << std::endl;

    std::cout << "------" << std::endl;

    

    // Write token / error files  
    std::ofstream tokenOutputFile(_TEST_DIR + fileInput + ".outlextokens", std::ofstream::out);
    std::ofstream errorOutputFile(_TEST_DIR + fileInput + ".outlexerrors", std::ofstream::out);

    tokenOutputFile.close();
    errorOutputFile.close();

    while (true);

    return 0;
}
