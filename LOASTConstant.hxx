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
    NodeKind kind() override {
        return NConstantBoolean; }
    void dump() override;
    bool value;
};

class NodeConstantDouble : public NodeConstant {
public:
    NodeConstantDouble(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() override {
        return NConstantDouble; }
    void dump() override;
    double value;
};

class NodeConstantInteger : public NodeConstant {
public:
    NodeConstantInteger(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() override {
        return NConstantInteger; }
    void dump() override;
    int64_t value;
};

class NodeConstantString : public NodeConstant {
public:
    NodeConstantString(std::shared_ptr<Node> parent)
            : NodeConstant(parent) { }
    NodeKind kind() override {
        return NConstantString; }
    void dump() override;
    std::string value;
};

}
}

#endif // VMFOROLIVE_LOASTCONSTANT_HXX
