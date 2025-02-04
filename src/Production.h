#pragma once
#include <string>
#include <vector>
#include "Token.h"

class Production
{
public:
	
	Production(std::string name, std::vector<Token*> firstSet, std::vector<Token*> followSet);

	std::string m_name; 
	std::vector<Token*> m_firstSet;
	std::vector<Token*> m_followSet;

private:



	

};

