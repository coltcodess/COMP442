#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"

static std::string _TEST_DIR = ".\\data\\";

int main()
{
    std::stringstream* buffer = new std::stringstream;
    std::ifstream srcFile(_TEST_DIR + "TEST_01.txt");


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

    std::cout << lexer->getNextLine() << std::endl;

    std::cout << "------" << std::endl;

    std::cout << lexer->getNextLine() << std::endl;
    std::cout << lexer->getNextLine() << std::endl;


    return 0;
}
