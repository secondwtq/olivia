//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOHLASTVISITOR_HXX
#define VMFOROLIVE_LOHLASTVISITOR_HXX

#include "LOHLBuilder.hxx"
#include "LOASTVisitor.hxx"
#include "LOHLBasic.hxx"
#include "LOAST.hxx"

namespace Olivia {
namespace HL {

class HLEmitterVisitor : public LOASTVisitorEmpty {
public:

    HLEmitterVisitor(std::shared_ptr<HLBlockBuilder> builder)
            : m_builder(builder) { }

    void visit(AST::NodePrefixUnaryExpression *node) override { }
    void visit(AST::NodeParenthesisExpression* node) override { }
    void visit(AST::NodeIdentifier* node) override;
    void visit(AST::NodeMemberExpression* node) override { }
    void visit(AST::NodeBinaryExpression* node) override { }
    void visit(AST::NodeCallExpression* node) override;
    void visit(AST::NodeStatementExpression* node) override;
    void visit(AST::NodeStatementReturn* node) override;
    void visit(AST::NodeBlock* node) override;
    void visit(AST::NodeStatementIf* node) override;
    void visit(AST::NodeStatementWhile *node) override;

    void visit(AST::NodeTypeSpec* node) override { }
    void visit(AST::NodeStatementVar* node) override;
    void visit(AST::MiscVarDeclarationList* node) override;
    void visit(AST::NodeDeclarationVar* node) override;

    void visit(AST::NodeDeclarationClassElementState* node) override { }
    void visit(AST::NodeDeclarationClassElementMethod* node) override { }
    void visit(AST::NodeDeclarationClass* node) override { }
    void visit(AST::NodeDeclarationParameter* node) override { }
    void visit(AST::NodeDeclarationSignature* node) override { }
    void visit(AST::NodeDeclarationFunction* node) override;
    void visit(AST::NodeDeclarationExternFunction* node) override;

    void visit(AST::NodeConstantBoolean *node) override;
    void visit(AST::NodeConstantDouble *node) override;
    void visit(AST::NodeConstantInteger *node) override;
    void visit(AST::NodeConstantString *node) override { }

    std::shared_ptr<HLBlockBuilder> builder() {
        return m_builder; }

private:
    std::shared_ptr<HLBlockBuilder> m_builder;
};

}
}

#endif // VMFOROLIVE_LOHLASTVISITOR_HXX
