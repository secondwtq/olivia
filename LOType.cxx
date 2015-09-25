//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOType.hxx"

#include <memory>

#include "LOValue.hxx"
#include "LOAST.hxx"
#include "LOScript.hxx"
#include "LOCell.hxx"

namespace Olivia {

std::shared_ptr<OliveType> convertTypeSpecNodeToOliveType(
        const AST::NodeTypeSpec& type, LOScript &script) {
    std::shared_ptr<OliveType> ret = std::make_shared<OliveType>();
    ret->setBaseType(type.base_type);
    if (type.concrete_type) {
        ret->setConcreteName(type.concrete_type->name); }
    ret->findConcrete(&script);
    return ret;
}

std::shared_ptr<OliveType> convertSignatureToOliveType(const AST::NodeDeclarationSignature& type, LOScript& script) {
    std::shared_ptr<OliveType> ret = std::make_shared<OliveType>();
    ret->setBaseType(TypeFunction);
    ret->setCallable(true);
    ret->callReturnType = convertTypeSpecNodeToOliveType(*type.return_type, script);
    for (auto arg : type.parameters) {
        auto argtype = convertTypeSpecNodeToOliveType(*arg->ptype, script);
        argtype->callArguments.push_back(argtype);
    }
    return ret;
}

std::string convertOliveTypeToString(const OliveType& type) {
    std::string ret = convertValueTypeToString(type.baseType());
    if (type.baseType() == TypeObject) {
        ret += type.concreteType()->name; }
    return ret;
}

}
