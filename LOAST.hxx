//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOAST_HXX
#define VMFOROLIVE_LOAST_HXX

#include "LOValue.hxx"
#include "LOLexer.hxx"
#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace Olivia {
namespace AST {

enum NodeKind {
    NUnknown,
    NBaseNode,

    NScript,

    NIdentifier,
    NTypeSpec,
    NDeclarationClass,
    NDeclarationClassElementState,
    NDeclarationClassElementMethod,
    NStatementVar,
    NVarDeclarationList,
    NDeclarationVar,

    NDeclarationFunction,
    NDeclarationSignature,
    NDeclarationParameter,
    NStatementBlock,
    NStatementExpression,
    NDeclarationExternFunction,

    NConstantBoolean,
    NConstantDouble,
    NConstantInteger,
    NConstantString,

    NPrefixUnaryExpression,
    NBinaryExpression,
    NParenthesisExpression,
    NCallExpression,
    NMemberExpression
};

class Node {
public:

    Node() : Node(nullptr) { }
    Node(std::shared_ptr<Node> parent) : m_parent(parent) { }

    // temporarily empty implementation
    virtual void generate_code() { }
    virtual ~Node() { }

    virtual void dump();

    virtual NodeKind kind() = 0;

    void dump_output(const std::string& msg);

    void set_parent(std::shared_ptr<Node> parent) {
        m_parent = parent; }

    std::shared_ptr<Node> parent() {
        return m_parent.lock(); }

private:
    std::weak_ptr<Node> m_parent;
};

class NodeScript : public Node {
public:
    NodeScript() : Node(nullptr) { }
    NodeKind kind() override {
        return NScript; }
    void dump() override { }
};

class NodeExpression : public Node {
public:
    NodeExpression(std::shared_ptr<Node> parent) : Node(parent) { }
    virtual bool isLHS() const {
        return false; }
};

class NodeUnaryExpression : public NodeExpression {
public:
    NodeUnaryExpression(std::shared_ptr<Node> parent) : NodeExpression(parent) { }
};

class NodePrefixUnaryExpression : public NodeUnaryExpression {
public:
    NodePrefixUnaryExpression(std::shared_ptr<Node> parent) : NodeUnaryExpression(parent) { }
    NodeKind kind() override {
        return NPrefixUnaryExpression; }
    std::shared_ptr<NodeExpression> operand;
    LexerTokenType op;
};

class NodePostfixExpression : public NodeUnaryExpression {
public:
    NodePostfixExpression(std::shared_ptr<Node> parent) : NodeUnaryExpression(parent) { }
};

class NodeLHSExpression : public NodePostfixExpression {
public:
    NodeLHSExpression(std::shared_ptr<Node> parent) : NodePostfixExpression(parent) { }
    bool isLHS() const override {
        return true; }
};

class NodePrimaryExpression : public NodeLHSExpression {
public:
    NodePrimaryExpression(std::shared_ptr<Node> parent) : NodeLHSExpression(parent) { }
};

// needed for keeping the object chain based on <PrimaryExpression>
class NodeParenthesisExpression : public NodePrimaryExpression {
public:
    NodeParenthesisExpression(std::shared_ptr<Node> parent)
            : NodePrimaryExpression(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NParenthesisExpression; }
    std::shared_ptr<NodeExpression> expression;
};

class NodeIdentifier : public NodePrimaryExpression {
public:
    NodeIdentifier(std::shared_ptr<Node> parent, const std::string& n)
            : NodePrimaryExpression(parent), name(n) { }

    void dump() override;

    NodeKind kind() override {
        return NIdentifier; }

    std::string name;
};

class NodeMemberExpression : public NodeLHSExpression {
public:
    NodeMemberExpression(std::shared_ptr<Node> parent)
        : NodeLHSExpression(parent) { }

    NodeKind kind() override {
        return NMemberExpression; }
    void dump() override;
    std::shared_ptr<NodeLHSExpression> expression;
    std::shared_ptr<NodeIdentifier> name;
};

class NodeBinaryExpression : public NodeExpression {
public:
    NodeBinaryExpression(std::shared_ptr<Node> parent) : NodeExpression(parent) { }
    NodeKind kind() override {
        return NBinaryExpression; }
    void dump() override;
    std::shared_ptr<NodeExpression> lhs, rhs;
    LexerTokenType op;
};

class NodeCallExpression : public NodeLHSExpression {
public:
    NodeCallExpression(std::shared_ptr<Node> parent) : NodeLHSExpression(parent) { }
    NodeKind kind() override {
        return NCallExpression; }
    void dump() override;
    std::shared_ptr<NodeLHSExpression> callee;
    std::vector<std::shared_ptr<NodeExpression>> arguments;
};

class NodeStatement : public Node {
public:
    NodeStatement(std::shared_ptr<Node> parent) : Node(parent) { }
    std::shared_ptr<NodeStatement> next_statement;
};

class NodeStatementExpression : public NodeStatement {
public:
    NodeStatementExpression(std::shared_ptr<Node> parent) : NodeStatement(parent) { }
    std::shared_ptr<NodeExpression> expression;
    NodeKind kind() override {
        return NStatementExpression; }
    void dump() override;
};

class NodeDeclaration : public NodeStatement {
public:
    NodeDeclaration(std::shared_ptr<Node> parent)
            : NodeStatement(parent) { }
    std::shared_ptr<NodeIdentifier> name;
};

class NodeTypeSpec : public Node {
public:
    NodeTypeSpec(std::shared_ptr<Node> parent)
            : Node(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NTypeSpec; }
    LOValueType base_type;
    std::shared_ptr<NodeIdentifier> concrete_type;
};

class MiscVarDeclarationList;

class NodeStatementVar : public NodeStatement {
public:
    NodeStatementVar(std::shared_ptr<Node> parent) : NodeStatement(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NStatementVar; }
    std::shared_ptr<MiscVarDeclarationList> list;
};

class NodeDeclarationVar : public NodeDeclaration {
public:
    NodeDeclarationVar(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NDeclarationVar; }
    std::shared_ptr<NodeTypeSpec> type;
    std::shared_ptr<NodeExpression> initializer;
};

class MiscVarDeclarationList : public Node {
public:
    MiscVarDeclarationList(std::shared_ptr<Node> parent)
        : Node(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NVarDeclarationList; }
    std::vector<std::shared_ptr<NodeDeclarationVar>> vars;
};

class NodeDeclarationClassElement : public NodeDeclaration {
public:
    NodeDeclarationClassElement(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }
    void dump() override;
    // type of variable(state) / return type of method
    std::shared_ptr<NodeTypeSpec> type;
};

class NodeDeclarationClassElementState : public NodeDeclarationClassElement {
public:
    NodeDeclarationClassElementState(std::shared_ptr<Node> parent)
        : NodeDeclarationClassElement(parent) { }
    NodeKind kind() override {
        return NDeclarationClassElementState; }
    void dump() override;
};

class NodeDeclarationClassElementMethod : public NodeDeclarationClassElement {
public:
    NodeDeclarationClassElementMethod(std::shared_ptr<Node> parent)
        : NodeDeclarationClassElement(parent) { }
    NodeKind kind() override {
        return NDeclarationClassElementMethod; }
    void dump() override;
};

class NodeDeclarationClass : public NodeDeclaration {
public:
    NodeDeclarationClass(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }
    void dump() override;
    NodeKind kind() override {
        return NDeclarationClass; }
    std::vector<std::shared_ptr<NodeDeclarationClassElement>> members;
};

class NodeDeclarationParameter : public NodeDeclaration {
public:

    NodeDeclarationParameter(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }

    NodeKind kind() override {
        return NDeclarationParameter; }
    void dump() override;

    std::shared_ptr<NodeIdentifier> name;
    std::shared_ptr<NodeTypeSpec> type;
};

class NodeDeclarationSignature : public NodeDeclaration {
public:
    NodeDeclarationSignature(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }

    NodeKind kind() override {
        return NDeclarationSignature; }
    void dump() override;

    std::vector<std::shared_ptr<NodeDeclarationParameter>> parameters;
    std::shared_ptr<NodeTypeSpec> return_type;
};

class NodeBlock : public NodeStatement {
public:
    NodeBlock(std::shared_ptr<Node> parent)
        : NodeStatement(parent) { }
    NodeKind kind() override {
        return NStatementBlock; }
    void dump() override;
    std::vector<std::shared_ptr<NodeStatement>> statements;
};

class NodeDeclarationFunction : public NodeDeclaration {
public:
    NodeDeclarationFunction(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }

    void dump() override;
    NodeKind kind() override {
        return NDeclarationFunction; }
    std::shared_ptr<NodeDeclarationSignature> signature;
    std::shared_ptr<NodeBlock> body;
};

class NodeDeclarationExternFunction : public NodeDeclaration {
public:
    NodeDeclarationExternFunction(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }

    void dump() override;
    NodeKind kind() override {
        return NDeclarationExternFunction; }
    std::shared_ptr<NodeDeclarationSignature> signature;
};

}
}

#include "LOASTConstant.hxx"

#endif // VMFOROLIVE_LOAST_HXX
