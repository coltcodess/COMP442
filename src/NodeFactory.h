#pragma once
#include <string>

/////////////////////////////////////////////////////////////////////

class Node
{
private:
	std::string type;
public:
	virtual std::string getType() = 0;
	virtual ~Node() = 0;

};

/////////////////////////////////////////////////////////////////////

class id_Node : public Node
{
public:
	std::string getType() {return "id";}

};

/////////////////////////////////////////////////////////////////////

class intNum_Node : public Node
{
public:
	std::string getType() { return "intNum"; }

};

/////////////////////////////////////////////////////////////////////

enum Type {id, intNum};

class NodeFactory
{
public:
	NodeFactory()
	{
		std::cout << "Node Factory created....." << std::endl;
	}

	Node* makeNode(Type t)
	{
		switch (t)
		{
		case id:
			return new id_Node();
		case intNum:
			return new intNum_Node();
		default: 
			return nullptr;
		}
	}


};