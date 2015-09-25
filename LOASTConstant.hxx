//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/13.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOASTCONSTANT_HXX
#define VMFOROLIVE_LOASTCONSTANT_HXX

#include "LOAST.hxx"

namespace Olivia {
namespace AST {

class NodeConstant : public NodePrimaryExpression {
public:
    NodeConstant(std::shared_ptr<Node> parent)
            : NodePrimaryExpression(parent) { }
};

class NodeConstantBoolean : public NodeConstant {
public:
    NodeConstantBoolean(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() const override {
        return NConstantBoolean; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    bool value;

    std::shared_ptr<OliveType> type() const override {
            return std::make_shared<OliveType>(TypeBoolean); }
    void accept(LOASTVisitor *visitor) override;
};

class NodeConstantDouble : public NodeConstant {
public:
    NodeConstantDouble(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() const override {
        return NConstantDouble; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    double value;

    std::shared_ptr<OliveType> type() const override {
        return std::make_shared<OliveType>(TypeDouble); }
    void accept(LOASTVisitor *visitor) override;
};

class NodeConstantInteger : public NodeConstant {
public:
    NodeConstantInteger(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() const override {
        return NConstantInteger; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    int64_t value;

    std::shared_ptr<OliveType> type() const;
    void accept(LOASTVisitor *visitor) override;
};

class NodeConstantString : public NodeConstant {
public:
    NodeConstantString(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() const override {
        return NConstantString; }
    void dump() const override;
    llvm::Value *generate_code(LOLModule *module) override;
    std::string value;

    std::shared_ptr<OliveType> type() const {
        return std::make_shared<OliveType>(TypeRawString); }
    void accept(LOASTVisitor *visitor) override;
};

}
}

#endif // VMFOROLIVE_LOASTCONSTANT_HXX
