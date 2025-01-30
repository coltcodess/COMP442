#include "Parser.h"

Parser::Parser(const std::string fileName) : m_sourceFileName(fileName)
{
	std::cout << "Create Parser..." << std::endl;

	std::ofstream outDev(m_sourceFileName + ".outderivation", std::ofstream::out);
	std::ofstream outErrors(m_sourceFileName + ".outsyntaxerrors", std::ofstream::out);





	// Close Files 
	m_derivationFile = &outDev;
	m_syntaxErrorsFile = &outErrors;

	m_derivationFile->close();
	m_syntaxErrorsFile->close();

	m_derivationFile = nullptr;
	m_syntaxErrorsFile = nullptr; 

}
