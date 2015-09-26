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
#include "LOASTVisitor.hxx"
#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace llvm {
class Value;
class Function;
}

namespace Olivia {
class LOASTVisitor;
class LOLModule;
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
    NMemberExpression,

    NStatementIf,
    NStatementWhile,
    NStatementReturn
};

class Node {
public:

    Node() : Node(nullptr) { }
    Node(std::shared_ptr<Node> parent) : m_parent(parent) { }

    // temporarily empty implementation
    virtual llvm::Value *generate_code(LOLModule *module) {
        printf("invalid code generation operation.\n");
        return nullptr;
    }

    virtual ~Node() { }

    virtual void dump() const;
    virtual NodeKind kind() const = 0;
    void dump_output(const std::string& msg) const;

    void set_parent(std::shared_ptr<Node> parent) {
        m_parent = parent; }

    const std::shared_ptr<Node> parent() const {
        return m_parent.lock(); }
    std::shared_ptr<Node> parent() {
        return m_parent.lock(); }

    virtual void accept(LOASTVisitor *visitor);

    virtual std::shared_ptr<OliveType> type() const = 0;

private:
    std::weak_ptr<Node> m_parent;
};

class NodeScript : public Node {
public:
    NodeScript() : Node(nullptr) { }
    NodeKind kind() const override {
        return NScript; }
    void dump() const override { }

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeNone); }
};

class NodeExpression : public Node {
public:
    NodeExpression(std::shared_ptr<Node> parent)
            : Node(parent) { }
    virtual bool isLHS() const {
        return false; }
    void accept(LOASTVisitor *visitor) override;
};

class NodeUnaryExpression : public NodeExpression {
public:
    NodeUnaryExpression(std::shared_ptr<Node> parent) : NodeExpression(parent) { }
};

class NodePrefixUnaryExpression : public NodeUnaryExpression {
public:
    NodePrefixUnaryExpression(std::shared_ptr<Node> parent) : NodeUnaryExpression(parent) { }
    NodeKind kind() const override {
        return NPrefixUnaryExpression; }
    std::shared_ptr<NodeExpression> operand;
    LexerTokenType op;

    void accept(LOASTVisitor *visitor) override;
    std::shared_ptr<OliveType> type() const override;
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
    void dump() const override;
    NodeKind kind() const override {
        return NParenthesisExpression; }
    std::shared_ptr<NodeExpression> expression;

    std::shared_ptr<OliveType> type() const override {
        return expression->type(); }
    void accept(LOASTVisitor *visitor) override;
};

class NodeIdentifier : public NodePrimaryExpression {
public:
    NodeIdentifier(std::shared_ptr<Node> parent, const std::string& n)
            : NodePrimaryExpression(parent), name(n) { }

    void dump() const override;

    llvm::Value *generate_code(LOLModule *module) override;
    NodeKind kind() const override {
        return NIdentifier; }

    std::string name;

    std::shared_ptr<OliveType> type() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeMemberExpression : public NodeLHSExpression {
public:
    NodeMemberExpression(std::shared_ptr<Node> parent)
        : NodeLHSExpression(parent) { }

    NodeKind kind() const override {
        return NMemberExpression; }
    void dump() const override;
    std::shared_ptr<NodeLHSExpression> expression;
    std::shared_ptr<NodeIdentifier> name;

    std::shared_ptr<OliveType> type() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeBinaryExpression : public NodeExpression {
public:
    NodeBinaryExpression(std::shared_ptr<Node> parent) : NodeExpression(parent) { }
    NodeKind kind() const override {
        return NBinaryExpression; }
    void dump() const override;
    std::shared_ptr<NodeExpression> lhs, rhs;
    llvm::Value *generate_code(LOLModule *module) override;
    LexerTokenType op;

    std::shared_ptr<OliveType> type() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeCallExpression : public NodeLHSExpression {
public:
    NodeCallExpression(std::shared_ptr<Node> parent) : NodeLHSExpression(parent) { }
    NodeKind kind() const override {
        return NCallExpression; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    std::shared_ptr<NodeLHSExpression> callee;
    std::vector<std::shared_ptr<NodeExpression>> arguments;

    std::shared_ptr<OliveType> type() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeStatement : public Node {
public:
    NodeStatement(std::shared_ptr<Node> parent) : Node(parent) { }
    std::shared_ptr<NodeStatement> next_statement;

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeNone); }
};

class NodeStatementExpression : public NodeStatement {
public:
    NodeStatementExpression(std::shared_ptr<Node> parent) : NodeStatement(parent) { }
    std::shared_ptr<NodeExpression> expression;
    NodeKind kind() const override {
        return NStatementExpression; }
    llvm::Value *generate_code(LOLModule *module) override;
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeStatementReturn : public NodeStatementExpression {
public:
    NodeStatementReturn(std::shared_ptr<Node> parent) : NodeStatementExpression(parent) { }
//    std::shared_ptr<NodeExpression> expression;
    NodeKind kind() const override {
        return NStatementReturn; }
    llvm::Value *generate_code(LOLModule *module) override;
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeBlock : public NodeStatement {
public:
    NodeBlock(std::shared_ptr<Node> parent)
            : NodeStatement(parent) { }
    NodeKind kind() const override {
        return NStatementBlock; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    std::vector<std::shared_ptr<NodeStatement>> statements;
    void accept(LOASTVisitor *visitor) override;
};

class NodeStatementIf : public NodeStatement {
public:
    NodeStatementIf(std::shared_ptr<Node> parent) : NodeStatement(parent) { }
    std::shared_ptr<NodeExpression> cond_;
    std::shared_ptr<NodeStatement> then_;
    std::shared_ptr<NodeStatement> else_;
    bool hasElse() const {
        return else_ != nullptr; }
    llvm::Value *generate_code(LOLModule *module) override;
    NodeKind kind() const override {
        return NStatementIf; }
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeStatementWhile : public NodeStatement {
public:
    NodeStatementWhile(std::shared_ptr<Node> parent)
            : NodeStatement(parent) { }
    std::shared_ptr<NodeExpression> cond;
    std::shared_ptr<NodeStatement> body;

    NodeKind kind() const override {
        return NStatementWhile; }
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclaration : public NodeStatement {
public:
    NodeDeclaration(std::shared_ptr<Node> parent)
            : NodeStatement(parent) { }
    std::shared_ptr<NodeIdentifier> name;

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeNone); };
    void accept(LOASTVisitor *visitor) override;
};

class NodeTypeSpec : public Node {
public:
    NodeTypeSpec(std::shared_ptr<Node> parent)
            : Node(parent) { }
    void dump() const override;
    NodeKind kind() const override {
        return NTypeSpec; }
    LOValueType base_type;
    std::shared_ptr<NodeIdentifier> concrete_type;

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeNone); }
    void accept(LOASTVisitor *visitor) override;
};

class MiscVarDeclarationList;

class NodeStatementVar : public NodeStatement {
public:
    NodeStatementVar(std::shared_ptr<Node> parent) : NodeStatement(parent) { }
    void dump() const override;
    NodeKind kind() const override {
        return NStatementVar; }

    llvm::Value *generate_code(LOLModule *module) override;
    std::shared_ptr<MiscVarDeclarationList> list;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationVar : public NodeDeclaration {
public:
    NodeDeclarationVar(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }
    void dump() const override;
    NodeKind kind() const override {
        return NDeclarationVar; }
    llvm::Value *generate_code(LOLModule *module) override;
    std::shared_ptr<NodeTypeSpec> type;
    std::shared_ptr<NodeExpression> initializer;
    void accept(LOASTVisitor *visitor) override;
};

class MiscVarDeclarationList : public Node {
public:
    MiscVarDeclarationList(std::shared_ptr<Node> parent)
        : Node(parent) { }
    void dump() const override;
    NodeKind kind() const override {
        return NVarDeclarationList; }
    llvm::Value *generate_code(LOLModule *module) override;
    std::vector<std::shared_ptr<NodeDeclarationVar>> vars;
    void accept(LOASTVisitor *visitor) override;

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeNone); }
};

class NodeDeclarationClassElement : public NodeDeclaration {
public:
    NodeDeclarationClassElement(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }
    void dump() const override;
    // type of variable(state) / return type of method
    std::shared_ptr<NodeTypeSpec> type;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationClassElementState : public NodeDeclarationClassElement {
public:
    NodeDeclarationClassElementState(std::shared_ptr<Node> parent)
        : NodeDeclarationClassElement(parent) { }
    NodeKind kind() const override {
        return NDeclarationClassElementState; }
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationClassElementMethod : public NodeDeclarationClassElement {
public:
    NodeDeclarationClassElementMethod(std::shared_ptr<Node> parent)
        : NodeDeclarationClassElement(parent) { }
    NodeKind kind() const override {
        return NDeclarationClassElementMethod; }
    void dump() const override;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationClass : public NodeDeclaration {
public:
    NodeDeclarationClass(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }
    void dump() const override;
    NodeKind kind() const override {
        return NDeclarationClass; }
    std::vector<std::shared_ptr<NodeDeclarationClassElement>> members;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationParameter : public NodeDeclaration {
public:

    NodeDeclarationParameter(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }

    NodeKind kind() const override {
        return NDeclarationParameter; }
    void dump() const override;

    std::shared_ptr<NodeIdentifier> name;
    std::shared_ptr<NodeTypeSpec> ptype;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationSignature : public NodeDeclaration {
public:
    NodeDeclarationSignature(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }

    NodeKind kind() const override {
        return NDeclarationSignature; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    std::string trusted_name;

    std::vector<std::shared_ptr<NodeDeclarationParameter>> parameters;
    std::shared_ptr<NodeTypeSpec> return_type;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationFunction : public NodeDeclaration {
public:
    NodeDeclarationFunction(std::shared_ptr<Node> parent)
        : NodeDeclaration(parent) { }

    void dump() const override;
    NodeKind kind() const override {
        return NDeclarationFunction; }
    llvm::Value *generate_code(LOLModule *module) override;
    std::shared_ptr<NodeDeclarationSignature> signature;
    std::shared_ptr<NodeBlock> body;
    void accept(LOASTVisitor *visitor) override;
};

class NodeDeclarationExternFunction : public NodeDeclaration {
public:
    NodeDeclarationExternFunction(std::shared_ptr<Node> parent)
            : NodeDeclaration(parent) { }

    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    NodeKind kind() const override {
        return NDeclarationExternFunction; }
    std::shared_ptr<NodeDeclarationSignature> signature;
    void accept(LOASTVisitor *visitor) override;
};

}
}

#include "LOASTConstant.hxx"

#endif // VMFOROLIVE_LOAST_HXX
