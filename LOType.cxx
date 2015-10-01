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

#include <string>

namespace Olivia {

std::shared_ptr<OliveType> OliveType::typeVoid = std::make_shared<OliveType>(TypeVoid);
std::shared_ptr<OliveType> OliveType::typeBoolean = std::make_shared<OliveType>(TypeBoolean);

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
        ret->callArguments.push_back(argtype);
    }
    return ret;
}

std::string convertOliveTypeToString(const OliveType& type) {
    std::string ret = convertValueTypeToString(type.baseType());
    if (type.baseType() == TypeObject) {
        ret += type.concreteType()->name; }
    if (type.baseType() == TypeFunction) {
        ret += " (";
        for (size_t i = 0; i < type.callArguments.size(); i++) {
            if (i != 0) {
                ret += ", "; }
            ret += convertOliveTypeToString(*type.callArguments[i]);
        }
        ret += ") : " + convertOliveTypeToString(*type.callReturnType);
    }
    return ret;
}

const char *convertValueTypeToString(Olivia::LOValueType type) {
    switch (type) {
        case TypeVoid:
            return "void";
        case TypeBoolean:
            return "boolean";
        case TypeDouble:
            return "double";
        case TypeInt32:
            return "int32";
        case TypeUInt32:
            return "uint32";
        case TypeInt8:
            return "int8";
        case TypeUndefined:
            return "undefined";
        case TypeNull:
            return "null";
        case TypeString:
            return "String";
        case TypeObject:
            return "Object";
        case TypeFunction:
            return "Function";
        default:
            return "<unknown type>";
    }
}

bool OliveType::equalsEffectively(std::shared_ptr<OliveType> other) {
    LOValueType t = baseType();
    if (t == TypeInt8 || t == TypeInt32 || t == TypeUInt32 || t == TypeDouble ||
            t == TypeBoolean || t == TypeString || t == TypeRawString) {
        return t == other->baseType();
    }
    return false;
}

}
