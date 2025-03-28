#include "CodeGeneratorVisitor.h"

CodeGeneratorVisitor::CodeGeneratorVisitor(std::ofstream* output)
{
	m_output = output;

	for (int i = 12; i >= 1; i--)
	{
		registerPool.push_back("r" + std::to_string(i));
	}
}

std::string CodeGeneratorVisitor::getNewTempVarName()
{
	tempVarNum++;
	return "t" + std::to_string(tempVarNum);
}

void CodeGeneratorVisitor::visit(Node& node)
{

}

void CodeGeneratorVisitor::visit(prog_Node& node)
{


	*m_output << moonExecCode + moonDataCode;


}

void CodeGeneratorVisitor::visit(classDecl_Node& node)
{

}

void CodeGeneratorVisitor::visit(funcDef_Node& node)
{

}

void CodeGeneratorVisitor::visit(impleDef_Node& node)
{

}

void CodeGeneratorVisitor::visit(memDeclAttrib_Node& node)
{
}

void CodeGeneratorVisitor::visit(memDeclFunc_Node& node)
{
}

void CodeGeneratorVisitor::visit(fParam_Node& node)
{
}

void CodeGeneratorVisitor::visit(statBlock_Node& node)
{
}

void CodeGeneratorVisitor::visit(inheritList_Node& node)
{
}

void CodeGeneratorVisitor::visit(varDecl_Node& node)
{
	for (auto child : node.getChildren())
	{
		child->accept(*this);
	}

	std::string outputCode;

	if (node.getChild(Type::type)->token->convertTokenTypeToString().compare("float") == 0)
	{
		moonDataCode += "           % space for variable " + node.getChild(Type::idLit)->token->lexem + "\n";
		moonDataCode += node.getChild(Type::idLit)->token->lexem + "           " + "res 8\n";
	}
	else if (node.getChild(Type::type)->token->convertTokenTypeToString().compare("int") == 0)
	{
		moonDataCode += "           % space for variable " + node.getChild(Type::idLit)->token->lexem + "\n";
		moonDataCode += node.getChild(Type::idLit)->token->lexem + "           " + "res 4\n";
	}
	else if (node.getChild(Type::type)->token->convertTokenTypeToString().compare("id") == 0)
	{
		// Lookup class in class table
		

	}
	else
	{

	}

}

void CodeGeneratorVisitor::visit(assignOp_Node& node)
{
	//TODO 
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	node.localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	moonExecCode += " lw \n";
	//moonExecCode += " sw " + node.getChildren()[0].moonVarName + "(r0)," + node.localRegister + "\n";

	this->registerPool.push_back(node.localRegister);
}

void CodeGeneratorVisitor::visit(multiOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	node.localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	node.leftChildRegister = this->registerPool.back();
	this->registerPool.pop_back();

	node.rightChildRegister = this->registerPool.back();
	this->registerPool.pop_back();

	node.moonVarName = this->getNewTempVarName();

	moonExecCode += "           lw " + node.leftChildRegister + node.getChildren()[0]->moonVarName + "(r0)\n";
	moonExecCode += "           lw " + node.rightChildRegister + node.getChildren()[1]->moonVarName + "(r0)\n";
	moonExecCode += "           mul " + node.localRegister + "," + node.leftChildRegister + "," + node.rightChildRegister +"\n";


	// Clean up registers
	this->registerPool.push_back(node.leftChildRegister);
	this->registerPool.push_back(node.rightChildRegister);
	this->registerPool.push_back(node.localRegister);
}

void CodeGeneratorVisitor::visit(idLit_Node& node)
{
}

void CodeGeneratorVisitor::visit(intLit_Node& node)
{
}

void CodeGeneratorVisitor::visit(floatLit_Node& node)
{
}

void CodeGeneratorVisitor::visit(type_Node& node)
{
	
}

void CodeGeneratorVisitor::visit(assignStat_Node& node)
{

}



