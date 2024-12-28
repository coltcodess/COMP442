#include <fstream>
#include <iostream>
#include "Lexer.h"

static std::string _TEST_DIR = ".\\data\\";

int main()
{

    std::ifstream* srcFile = new std::ifstream(_TEST_DIR + "TEST_01.txt");

    if (srcFile->is_open())
    {
        std::cout << "DEBUG - Opened file" << std::endl;
    }
    else
    {
        std::cout << "ERROR - Failed to Open file" << std::endl;
        return 0;
    }

    Lexer* lexer = new Lexer(srcFile);

    if (lexer->isKeyword("int"))
    {
        std::cout << "Keyword" << std::endl;
    }

    if (lexer->isDigit('1'))
    {
        std::cout << "Number" << std::endl;
    }

    return 0;
}
