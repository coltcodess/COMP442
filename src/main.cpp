#include <fstream>
#include <iostream>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "NodeFactory.h"

const std::string SOURCE_FILE_TYPE = ".src";
const std::string OUTPUT_TOKEN_FILE_TYPE = ".outlextokens";


void printNodes(Node* root, int level  = 0)
{
    if (root == nullptr) {
        return;
    }

    // Print the current node's data
    // Indent according to the level of the node
    std::cout << std::string(level, ' ') << "| " << root->getType() << std::endl;

    // Print each child node, with a '|' prefix
    for (Node* child : root->children) {
        // Recursively print each child's children
        printNodes(child, level + 2);
    }

}

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
    //Lexer* lexer = new Lexer(buffer->str(), fileInput);
    //Parser* parser = new Parser(fileInput, *lexer);

    NodeFactory nodeFactory; 
 
    Node* n2 = nodeFactory.makeNode(Type::idLit);
    Node* n3 = nodeFactory.makeNode(Type::intLit);

    Node* n4 = nodeFactory.makeNode(Type::idLit);
    Node* n5 = nodeFactory.makeNode(Type::intLit);

    Node* root = nodeFactory.makeSubtree(nodeFactory.makeNode(Type::floatLit), 2, n2, n3);
    nodeFactory.makeSubtree(n3, 1, n4);
    nodeFactory.makeSubtree(n2, 1, n5);

   
  

    printNodes(root);

    return 0;
}


