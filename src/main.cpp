#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "NodeFactory.h"
#include "SymbolTableCreatorVistor.h"
#include "TypeCheckingVisitor.h"

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
    Parser* parser = new Parser(fileInput, *lexer);
    
    // Create Vistor
    // Assignment 4
    std::ofstream* outSemErrors = new std::ofstream(fileInput + ".outsemanticerrors", std::ofstream::out);
    std::ofstream* outSymbolTables = new std::ofstream(fileInput + ".outsymboltables", std::ofstream::out);

    SymbolTableCreatorVistor symbolTableCreatorVistor(outSymbolTables, outSemErrors);
    TypeCheckingVisitor typeCheckingVisitor(outSemErrors);

    Node* astRoot = parser->getASTroot();

    astRoot->accept(symbolTableCreatorVistor);
    symbolTableCreatorVistor.print();




    

    return 0;
}


