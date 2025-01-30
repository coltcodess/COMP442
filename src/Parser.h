#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"

class Parser
{

public:
	Parser(std::string fileName);

private:
	Token* m_current_token;

	std::string m_sourceFileName;

	std::ofstream* m_derivationFile;
	std::ofstream* m_syntaxErrorsFile;
};

