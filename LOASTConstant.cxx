//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/13.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOASTConstant.hxx"
#include "LOAST.hxx"

namespace Olivia {
namespace AST {

void NodeConstantBoolean::dump() const {
    std::string ret = "Expression - Constant - Boolean - ";
    ret += value ? "<true>" : "<false>";
    dump_output(ret);
}

void NodeConstantDouble::dump() const {
    std::string ret = "Expression - Constant - Double - <";
    ret += std::to_string(value) + ">";
    dump_output(ret);
}

void NodeConstantInteger::dump() const {
    std::string ret = "Expression - Constant - Integer - <";
    ret += std::to_string(value) + ">";
    dump_output(ret);
}

void NodeConstantString::dump() const {
    std::string ret = "Expression - Constant - String - <";
    ret += value + ">";
    dump_output(ret);
}

}
}
