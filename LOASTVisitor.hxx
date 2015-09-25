//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOASTVISITOR_HXX
#define VMFOROLIVE_LOASTVISITOR_HXX

namespace Olivia {

namespace AST {
class Node;

class NodePrefixUnaryExpression;
class NodeParenthesisExpression;
class NodeIdentifier;
class NodeMemberExpression;
class NodeBinaryExpression;
class NodeCallExpression;
class NodeStatementExpression;
class NodeStatementReturn;
class NodeBlock;
class NodeStatementIf;

class NodeTypeSpec;
class NodeStatementVar;
class NodeDeclarationVar;
class MiscVarDeclarationList;

class NodeDeclarationClassElementState;
class NodeDeclarationClassElementMethod;
class NodeDeclarationClass;
class NodeDeclarationParameter;
class NodeDeclarationSignature;
class NodeDeclarationFunction;
class NodeDeclarationExternFunction;

class NodeConstantBoolean;
class NodeConstantDouble;
class NodeConstantInteger;
class NodeConstantString;
}

class LOASTVisitor {
public:

    virtual void visit(AST::Node *node) = 0;

    virtual void visit(AST::NodePrefixUnaryExpression *node) = 0;
    virtual void visit(AST::NodeParenthesisExpression* node) = 0;
	virtual void visit(AST::NodeIdentifier* node) = 0;
	virtual void visit(AST::NodeMemberExpression* node) = 0;
	virtual void visit(AST::NodeBinaryExpression* node) = 0;
	virtual void visit(AST::NodeCallExpression* node) = 0;
	virtual void visit(AST::NodeStatementExpression* node) = 0;
	virtual void visit(AST::NodeStatementReturn* node) = 0;
	virtual void visit(AST::NodeBlock* node) = 0;
	virtual void visit(AST::NodeStatementIf* node) = 0;

	virtual void visit(AST::NodeTypeSpec* node) = 0;
	virtual void visit(AST::NodeStatementVar* node) = 0;
	virtual void visit(AST::NodeDeclarationVar* node) = 0;
	virtual void visit(AST::MiscVarDeclarationList* node) = 0;

	virtual void visit(AST::NodeDeclarationClassElementState* node) = 0;
	virtual void visit(AST::NodeDeclarationClassElementMethod* node) = 0;
	virtual void visit(AST::NodeDeclarationClass* node) = 0;
	virtual void visit(AST::NodeDeclarationParameter* node) = 0;
	virtual void visit(AST::NodeDeclarationSignature* node) = 0;
	virtual void visit(AST::NodeDeclarationFunction* node) = 0;
	virtual void visit(AST::NodeDeclarationExternFunction* node) = 0;

    virtual void visit(AST::NodeConstantBoolean *node) = 0;
    virtual void visit(AST::NodeConstantDouble *node) = 0;
    virtual void visit(AST::NodeConstantInteger *node) = 0;
    virtual void visit(AST::NodeConstantString *node) = 0;
};

class LOASTVisitorEmpty : public LOASTVisitor {
public:
    void visit(AST::Node *node) override { }

    void visit(AST::NodePrefixUnaryExpression *node) override { }
    void visit(AST::NodeParenthesisExpression* node) override { }
	void visit(AST::NodeIdentifier* node) override { }
	void visit(AST::NodeMemberExpression* node) override { }
	void visit(AST::NodeBinaryExpression* node) override { }
	void visit(AST::NodeCallExpression* node) override { }
	void visit(AST::NodeStatementExpression* node) override { }
	void visit(AST::NodeStatementReturn* node) override { }
	void visit(AST::NodeBlock* node) override { }
	void visit(AST::NodeStatementIf* node) override { }

	void visit(AST::NodeTypeSpec* node) override { }
	void visit(AST::NodeStatementVar* node) override { }
	void visit(AST::NodeDeclarationVar* node) override { }
	void visit(AST::MiscVarDeclarationList* node) override { }

	void visit(AST::NodeDeclarationClassElementState* node) override { }
	void visit(AST::NodeDeclarationClassElementMethod* node) override { }
	void visit(AST::NodeDeclarationClass* node) override { }
	void visit(AST::NodeDeclarationParameter* node) override { }
	void visit(AST::NodeDeclarationSignature* node) override { }
	void visit(AST::NodeDeclarationFunction* node) override { }
	void visit(AST::NodeDeclarationExternFunction* node) override { }

    void visit(AST::NodeConstantBoolean *node) override { }
    void visit(AST::NodeConstantDouble *node) override { }
    void visit(AST::NodeConstantInteger *node) override { }
    void visit(AST::NodeConstantString *node) override { }
};

}

#endif // VMFOROLIVE_LOASTVISITOR_HXX
