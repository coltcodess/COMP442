#include "Production.h"

Production::Production(std::string name, std::vector<Token*> firstSet, std::vector<Token*> followSet) :
	m_name(name), m_firstSet(firstSet), m_followSet(followSet)
{


}
