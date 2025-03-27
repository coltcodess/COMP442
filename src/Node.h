#pragma once
#include "Visitor.h"

/////////////////////////////////////////////////////////////////////

class null_Node : public Node
{
public:
	null_Node() {};
	virtual ~null_Node() {};

	void accept(Visitor& v) override {
		v.visit(*this);
	};

};

/////////////////////////////////////////////////////////////////////

class prog_Node : public Node
{
public:
	prog_Node(Type t) : Node(t)
	{ }

	virtual ~prog_Node() {};

	void accept(Visitor& visitor) override
	{
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class classDeclList_Node : public Node
{
public:
	classDeclList_Node(Type t) : Node(t)
	{ }

	virtual ~classDeclList_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
	
};

/////////////////////////////////////////////////////////////////////

class classDecl_Node : public Node
{
public:
	classDecl_Node(Type t) : Node(t)
	{ }

	virtual ~classDecl_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class assignStat_Node : public Node
{
public:
	assignStat_Node(Type t) : Node(t)
	{ }

	virtual ~assignStat_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class implDefList_Node : public Node
{
public:
	implDefList_Node(Type t) : Node(t)
	{ }

	virtual ~implDefList_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class impleDef_Node : public Node
{
public:
	impleDef_Node(Type t) : Node(t)
	{ }

	virtual ~impleDef_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class funcDefList_Node : public Node
{
public:
	funcDefList_Node(Type t) : Node(t)
	{ }

	virtual ~funcDefList_Node() {};

	void accept(Visitor& visitor) override {
	
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class funcDecl_Node : public Node
{
public:
	funcDecl_Node(Type t) : Node(t)
	{ }

	virtual ~funcDecl_Node() {};

	void accept(Visitor& visitor) override {
	
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class funcDef_Node : public Node
{
public:
	funcDef_Node(Type t) : Node(t)
	{ }
	
	virtual ~funcDef_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	
	};
};

/////////////////////////////////////////////////////////////////////

class inheritList_Node : public Node
{
public:
	inheritList_Node(Type t) : Node(t)
	{ }

	virtual ~inheritList_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class memDeclList_Node : public Node
{
public:
	memDeclList_Node(Type t) : Node(t)
	{ }

	virtual ~memDeclList_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class memDeclFunc_Node : public Node
{
public:
	memDeclFunc_Node(Type t) : Node(t)
	{ }

	virtual ~memDeclFunc_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////


class memDeclAttrib_Node : public Node
{
public:
	memDeclAttrib_Node(Type t) : Node(t)
	{ }

	virtual ~memDeclAttrib_Node() {};

	void accept(Visitor& visitor) override 
	{
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class fParamsList_Node : public Node
{
public:
	fParamsList_Node(Type t) : Node(t)
	{ }

	virtual ~fParamsList_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class fParam_Node : public Node
{
public:
	fParam_Node(Type t) : Node(t)
	{ }

	virtual ~fParam_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class arraySizeList_Node : public Node
{
public:
	arraySizeList_Node(Type t) : Node(t)
	{ }

	virtual ~arraySizeList_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class statBlock_Node : public Node
{
public:
	statBlock_Node(Type t) : Node(t)
	{ }

	virtual ~statBlock_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class returnStat_Node : public Node
{
public:
	returnStat_Node(Type t) : Node(t)
	{ }

	virtual ~returnStat_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class whileStat_Node : public Node
{
public:
	whileStat_Node(Type t) : Node(t)
	{ }

	virtual ~whileStat_Node() {};

	void accept(Visitor& visitor) override {
		
		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class writeStat_Node : public Node
{
public:
	writeStat_Node(Type t) : Node(t)
	{ }

	virtual ~writeStat_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class readStat_Node : public Node
{
public:
	readStat_Node(Type t) : Node(t)
	{ }

	virtual ~readStat_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class ifStat_Node : public Node
{
public:
	ifStat_Node(Type t) : Node(t)
	{ }

	virtual ~ifStat_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class expr_Node : public Node
{
public:
	expr_Node(Type t) : Node(t)
	{ }

	virtual ~expr_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class relExpr_Node : public Node
{
public:
	relExpr_Node(Type t) : Node(t)
	{ }

	virtual ~relExpr_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class varDecl_Node : public Node
{
public:
	varDecl_Node(Type t) : Node(t)
	{ }

	virtual ~varDecl_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class assignOp_Node : public Node
{
public:
	assignOp_Node(Type t) : Node(t)
	{ }

	virtual ~assignOp_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class fCall_Node: public Node
{
public:
	fCall_Node(Type t) : Node(t)
	{ }

	virtual ~fCall_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class aParams_Node : public Node
{
public:
	aParams_Node(Type t) : Node(t)
	{ }

	virtual ~aParams_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class relOp_Node : public Node
{
public:
	relOp_Node(Type t) : Node(t)
	{ }

	virtual ~relOp_Node() {};

	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class addOp_Node : public Node
{
public:
	addOp_Node(Type t) : Node(t)
	{ }

	virtual ~addOp_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class multiOp_Node : public Node
{
public:
	multiOp_Node(Type t) : Node(t)
	{ }

	virtual ~multiOp_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class dot_Node : public Node
{
public:
	dot_Node(Type t) : Node(t)
	{ }

	virtual ~dot_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class dataMem_Node : public Node
{
public:
	dataMem_Node(Type t) : Node(t)
	{ }

	virtual ~dataMem_Node() {};

	void accept(Visitor& visitor) override {

		for (Node* child : this->getChildren())
		{
			child->accept(visitor);
		}

		visitor.visit(*this);
	};
};


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class idLit_Node : public Node
{
public:
	idLit_Node(Type t) : Node(t)
	{ }

	virtual ~idLit_Node() {};

	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	};

};

/////////////////////////////////////////////////////////////////////

class type_Node : public Node
{
public:
	type_Node(Type t) : Node(t)
	{ }

	type_Node(Token& token, Type t) : Node(&token, t)
	{ }

	virtual ~type_Node() {};

	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////

class intLit_Node : public Node
{
public:
	intLit_Node(Type t) : Node(t)
	{ }

	virtual ~intLit_Node() {};

	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	};

};

/////////////////////////////////////////////////////////////////////

class floatLit_Node : public Node
{
public:
	floatLit_Node(Type t) : Node(t)
	{ }

	virtual ~floatLit_Node() {};

	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	};
};

/////////////////////////////////////////////////////////////////////
