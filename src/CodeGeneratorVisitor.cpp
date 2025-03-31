#include "CodeGeneratorVisitor.h"

std::string MOON_INDENT = "     ";

CodeGeneratorVisitor::CodeGeneratorVisitor(std::ofstream* output)
{
	m_output = output;

	// Initialize Registers 
	for (int i = 12; i >= 1; i--)
	{
		registerPool.push_back("r" + std::to_string(i));
	}

	// Program start point
	moonExecCode += "% start of program\n";
	moonExecCode += MOON_INDENT + "entry\n";
	moonExecCode += MOON_INDENT + "addi" + MOON_INDENT + "r14, r0, topaddr% Set stack pointer\n";

	// Start of program data 
	moonDataCode += "% start of data code\n";
	moonDataCode += "% buffer space used for console output\n";


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

	// End program
	moonExecCode += MOON_INDENT + "hlt\n";

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
		moonDataCode += "% space for variable " + node.getChild(Type::idLit)->token->lexem + "\n";
		moonDataCode += node.getChild(Type::idLit)->token->lexem + "           " + "res 8\n";
	}
	else if (node.getChild(Type::type)->token->convertTokenTypeToString().compare("int") == 0)
	{
		moonDataCode += MOON_INDENT + "% space for variable " + node.getChild(Type::idLit)->token->lexem + "\n";
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

	//moonExecCode += MOON_INDENT + "lw" + MOON_INDENT + node.localRegister + "," + node.getChildren()[0]->token->lexem + "(r0)\n";
	//moonExecCode += MOON_INDENT + "sw" + MOON_INDENT + node.getChildren()[0]->token->lexem + "(r0)," + node.localRegister + "\n";

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

	//moonExecCode += MOON_INDENT + "lw" + MOON_INDENT + node.leftChildRegister + "," + node.getChildren()[0]->token->lexem + "(r0)" + MOON_INDENT + "% " + node.leftChildRegister + " := " + node.getChildren()[0]->token->lexem + "\n";
	//moonExecCode += MOON_INDENT + "lw" + MOON_INDENT + node.rightChildRegister + "," + node.getChildren()[1]->token->lexem + "(r0)" + MOON_INDENT + "% " + node.rightChildRegister + " := " + node.getChildren()[1]->token->lexem + "\n";
	//moonExecCode += MOON_INDENT + "mul" + MOON_INDENT + node.localRegister + "," + node.leftChildRegister + "," + node.rightChildRegister +"\n";

	moonExecCode += MOON_INDENT + "sw" + MOON_INDENT + node.moonVarName + "(r0)," + node.localRegister + "\n";

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
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(assignStat_Node& node)
{

}


void CodeGeneratorVisitor::visit(addOp_Node& node)
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

	// Code generation
	//moonExecCode += MOON_INDENT + "lw     " + node.leftChildRegister + "," + node.getChildren()[0]->token->lexem + "(r0)" + MOON_INDENT + "% " + node.leftChildRegister + " := " + node.getChildren()[0]->token->lexem + "\n";
	//moonExecCode += MOON_INDENT + "lw     " + node.rightChildRegister + "," + node.getChildren()[1]->token->lexem + "(r0)" + MOON_INDENT + "% " + node.rightChildRegister + " := " + node.getChildren()[1]->token->lexem + "\n";
	//moonExecCode += MOON_INDENT + "add    " + node.localRegister + "," + node.leftChildRegister + "," + node.rightChildRegister + "\n";

	// Clean up registers
	this->registerPool.push_back(node.leftChildRegister);
	this->registerPool.push_back(node.rightChildRegister);
	this->registerPool.push_back(node.localRegister);
}

