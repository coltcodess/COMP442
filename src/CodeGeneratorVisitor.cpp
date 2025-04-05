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

}

void CodeGeneratorVisitor::visit(Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(prog_Node& node)
{
	// Program start point
	moonExecCode += "% start of program\n";
	moonExecCode += MOON_INDENT + "entry\n";
	moonExecCode += MOON_INDENT + "addi " + "r14, r0, topaddr % Set stack pointer\n";

	// Start of program data 
	moonDataCode += "% start of data code\n";
	moonDataCode += "% buffer space used for console output\n";

	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
	// End program

	moonDataCode += MOON_INDENT + "% buffer space used for console output\n";
	moonDataCode += "buf" + MOON_INDENT + "res 20\n";
	moonExecCode += MOON_INDENT + "hlt\n";

	*m_output << moonExecCode + moonDataCode;
}

void CodeGeneratorVisitor::visit(classDecl_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(funcDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}


void CodeGeneratorVisitor::visit(impleDef_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(memDeclAttrib_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(memDeclFunc_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(fParam_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(statBlock_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(inheritList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(varDecl_Node& node)
{
	for (auto child : node.getChildren())
	{
		child->accept(*this);
	}

	if (node.getChildren()[1]->getType() == Type::intLit)
	{
		moonDataCode += MOON_INDENT + "% space for variable " + node.getChildren()[1]->token->convertTokenTypeToString() + "\n";
		moonDataCode += node.token->lexem + MOON_INDENT + " res 4\n";
	}
}

void CodeGeneratorVisitor::visit(assignOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	std::string localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	moonExecCode += MOON_INDENT + "% processing: " + node.getChildren()[0]->moonVarName + " := " + node.getChildren()[1]->moonVarName + "\n";
	moonExecCode += MOON_INDENT + "lw " + localRegister + "," + node.getChildren()[1]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "sw " + node.getChildren()[0]->moonVarName + "(r0)," + localRegister + "\n";

	this->registerPool.push_back(localRegister);
}

void CodeGeneratorVisitor::visit(multiOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
	
	std::string localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	std::string leftRegister = this->registerPool.back();
	this->registerPool.pop_back();

	std::string rightRegister = this->registerPool.back();
	this->registerPool.pop_back();

	moonExecCode += MOON_INDENT + "% processing: " + node.moonVarName + " := " + node.getChildren()[0]->moonVarName + " + " + node.getChildren()[1]->moonVarName + "\n";
	moonExecCode += MOON_INDENT + "lw " + leftRegister + "," + node.getChildren()[0]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "lw " + rightRegister + "," + node.getChildren()[1]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "mul " + localRegister + "," + leftRegister + "," + rightRegister + "\n";
	moonDataCode += MOON_INDENT + "% space for " + node.getChildren()[0]->moonVarName + " + " + node.getChildren()[1]->moonVarName + "\n";
	moonDataCode += node.moonVarName + MOON_INDENT + " res 4\n";
	moonExecCode += MOON_INDENT + "sw " + node.moonVarName + "(r0)," + localRegister + "\n";


	this->registerPool.push_back(rightRegister);
	this->registerPool.push_back(leftRegister);
	this->registerPool.push_back(localRegister);
}

void CodeGeneratorVisitor::visit(idLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(intLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	std::string localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	moonDataCode += node.moonVarName + MOON_INDENT + "res 4\n";
	moonExecCode += MOON_INDENT + "addi " + localRegister + ",r0," + node.token->lexem + "\n";
	moonExecCode += MOON_INDENT + "sw " + node.moonVarName + "(r0)," + localRegister + "\n";
	// deallocate the register for the current node
	this->registerPool.push_back(localRegister);
}

void CodeGeneratorVisitor::visit(floatLit_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
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
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}

void CodeGeneratorVisitor::visit(writeStat_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	std::string localregister = this->registerPool.back();
	this->registerPool.pop_back();

	moonExecCode += MOON_INDENT + "% processing: put(" + node.getChildren()[0]->moonVarName + ")\n";
	moonExecCode += MOON_INDENT + "lw " + localregister + "," + node.getChildren()[0]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "sw -8(r14)," + localregister + "\n";
	moonExecCode += MOON_INDENT + "addi " + localregister + ",r0, buf\n";
	moonExecCode += MOON_INDENT + "sw -12(r14)," + localregister + "\n";
	moonExecCode += MOON_INDENT + "jl r15, intstr\n";
	moonExecCode += MOON_INDENT + "sw -8(r14),r13\n";
	moonExecCode += MOON_INDENT + "jl r15, putstr\n";
	
	//deallocate local register
	this->registerPool.push_back(localregister);
}

void CodeGeneratorVisitor::visit(funcDefList_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}
}


void CodeGeneratorVisitor::visit(addOp_Node& node)
{
	for (Node* child : node.getChildren())
	{
		child->accept(*this);
	}

	std::string localRegister = this->registerPool.back();
	this->registerPool.pop_back();

	std::string leftRegister = this->registerPool.back();
	this->registerPool.pop_back();

	std::string rightRegister = this->registerPool.back();
	this->registerPool.pop_back();

	moonExecCode += MOON_INDENT + "% processing: " + node.moonVarName + " := " + node.getChildren()[0]->moonVarName + " + " + node.getChildren()[1]->moonVarName + "\n";
	moonExecCode += MOON_INDENT + "lw " + leftRegister + "," + node.getChildren()[0]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "lw " + rightRegister + "," + node.getChildren()[1]->moonVarName + "(r0)\n";
	moonExecCode += MOON_INDENT + "add " + localRegister + "," + leftRegister + "," + rightRegister + "\n";
	moonDataCode += MOON_INDENT + "% space for " + node.getChildren()[0]->moonVarName + " + " + node.getChildren()[1]->moonVarName + "\n";
	moonDataCode += node.moonVarName + MOON_INDENT + " res 4\n";
	moonExecCode += MOON_INDENT + "sw " + node.moonVarName + "(r0)," + localRegister + "\n";


	this->registerPool.push_back(rightRegister);
	this->registerPool.push_back(leftRegister);
	this->registerPool.push_back(localRegister);

}

