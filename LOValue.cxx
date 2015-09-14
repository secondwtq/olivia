//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOValue.hxx"

void test() {
    Olivia::LOValue v;
}

namespace Olivia {

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
        default:
            return "<unknown type>";
    }
}

}
