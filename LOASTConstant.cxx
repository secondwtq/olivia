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
#include "LOASTVisitor.hxx"

#include <limits.h>

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

std::shared_ptr<OliveType> NodeConstantInteger::type() const {
    // TODO: more boundary checks
    if (value >= INT8_MIN && value <= INT8_MAX) {
        return std::make_shared<OliveType>(TypeInt8);
    } else if (value >= INT32_MIN && value <= INT32_MAX) {
        return std::make_shared<OliveType>(TypeInt32);
    } else if (value <= UINT32_MAX) {
        return std::make_shared<OliveType>(TypeUInt32); }
    return nullptr;
}

}

}
