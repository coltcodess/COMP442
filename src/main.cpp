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
    std::ifstream srcFile(_TEST_DIR + fileInput + ".txt");

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

    // Write token file 
    std::ofstream outputFile(_TEST_DIR + fileInput + "_output" + ".txt", std::ofstream::out);

    outputFile.close();

    while (true);

    return 0;
}
