//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOAST.hxx"
#include "LOASTVisitor.hxx"

namespace Olivia {
namespace AST {

void Node::dump() {
    dump_output("Node - Base");
}

void NodeIdentifier::dump() {
    dump_output("Identifier - " + name);
}

void NodeStatementVar::dump() {
    dump_output("Statement - 'var'");
    if (list) {
        list->dump();
    }
}

void NodeDeclarationVar::dump() {
    dump_output("Declaration - 'var'");
    if (name) {
        name->dump();
    }
    if (type) {
        type->dump();
    }
    if (initializer) {
        initializer->dump();
    }
}

void NodeTypeSpec::dump() {
    dump_output(std::string("Node - Type - ") + convertValueTypeToString(base_type));
    if (concrete_type) {
        concrete_type->dump();
    }
}

void NodeDeclarationClassElement::dump() {
    dump_output("Declaration - element of 'class'");
    if (name) {
        name->dump();
    }
    if (type) {
        type->dump();
    }
}

void NodeDeclarationClass::dump() {
    dump_output("Decleartion - 'class'");
    if (name) {
        name->dump();
    }
    for (auto member : members) {
        member->dump();
    }
}

void MiscVarDeclarationList::dump() {
    dump_output("Misc - 'var' declearation list");
    for (auto var : vars) {
        var->dump();
    }
}

void Node::dump_output(const std::string& msg) {
    if (!this->parent()) {
        printf("%s\n", msg.c_str());
    } else {
        this->parent()->dump_output(' ' + msg);
    }
}

void NodeDeclarationClassElementState::dump() {
    dump_output("Declaration - class element state");
    if (name) {
        name->dump();
    }
    if (type) {
        type->dump();
    }
}

void NodeDeclarationClassElementMethod::dump() {
    dump_output("Declaration - class element method");
    if (name) {
        name->dump();
    }
    if (type) {
        type->dump();
    }
}

void NodeDeclarationParameter::dump() {
    dump_output("Declaration - parameter");
    if (name) {
        name->dump();
    }
    if (type) {
        type->dump();
    }
}

void NodeDeclarationSignature::dump() {
    dump_output("Declaration - signature");
    for (auto parameter : parameters) {
        parameter->dump();
    }
    if (return_type) {
        return_type->dump();
    }
}

void NodeDeclarationFunction::dump() {
    dump_output("Declaration - 'function'");
    if (name) {
        name->dump();
    }
    if (signature) {
        signature->dump();
    }
    if (body) {
        body->dump();
    }
}

void NodeBlock::dump() {
    dump_output("Statement - block");
    for (auto statement : statements) {
        statement->dump();
    }
}

void NodeDeclarationExternFunction::dump() {
    dump_output("Declaration - 'extern function'");
    if (name) {
        name->dump();
    }
    if (signature) {
        signature->dump();
    }
}

void NodeBinaryExpression::dump() {
    std::string ret = "Expression - Binary - <";
    ret += op;
    ret += ">";
    dump_output(ret);
    if (lhs) {
        lhs->dump(); }
    if (rhs) {
        rhs->dump(); }
}

void NodeParenthesisExpression::dump() {
    dump_output("Expression - Parenthesis");
    if (expression) {
        expression->dump(); }
}

void NodeCallExpression::dump() {
    dump_output("Expression - Call");
    if (callee) {
        callee->dump(); }
    for (auto argument : arguments) {
        argument->dump(); }
}

void NodeMemberExpression::dump() {
    dump_output("Expression - Member Access");
    if (expression) {
        expression->dump(); }
    if (name) {
        name->dump(); }
}

void NodeStatementExpression::dump() {
    dump_output("Statement - Expression");
    if (expression) {
        expression->dump(); }
}

void NodeStatementReturn::dump() {
    dump_output("Statement - 'return'");
    if (expression) {
        expression->dump(); }
}

void Node::accept(LOASTVisitor *visitor) {

}

}
}
