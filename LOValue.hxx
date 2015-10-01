//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOVALUE_HXX
#define VMFOROLIVE_LOVALUE_HXX

#include <stdint.h>
#include <assert.h>

#include <string>
#include <memory>
#include <vector>

namespace Olivia {

enum LOValueType {
    TypeNone,

    TypeVoid,
    TypeBoolean,
    TypeDouble,
    TypeCell,
    TypeInt32,
    TypeUInt32,
    TypeInt8,
    TypeUndefined,
    TypeNull,
    TypeString,
    TypeRawString,
    TypeObject,

    TypeFunction
};


class LOClass;
class LOScript;

struct OliveType {
public:

    OliveType() : OliveType(TypeNone) { }
    OliveType(LOValueType base_type) :
            m_base_type(base_type) { }

    void setBaseType(LOValueType type) {
        m_base_type = type; }
    LOValueType baseType() const {
        return m_base_type; }

    const std::shared_ptr<LOClass> concreteType() const {
        return m_concrete; }

    std::shared_ptr<LOClass> concreteType() {
        return m_concrete; }

    bool finished() const {
        return ((baseType() != TypeObject) || (concreteType())); }

    bool findConcrete(LOScript *script);

    void setConcreteName(const std::string& name) {
        m_concrete_tmp = name; }

    void setCallable(bool callable) {
        m_callable = callable; }
    bool callable() const {
        return m_callable; }

    bool equalsEffectively(std::shared_ptr<OliveType> other);

    std::shared_ptr<OliveType> callReturnType;
    std::vector<std::shared_ptr<OliveType>> callArguments;

    static std::shared_ptr<OliveType> basicType(LOValueType type) {
        switch (type) {
            case TypeVoid: return typeVoid;
            case TypeBoolean: return typeBoolean;
            default:
                return nullptr;
        }
    }

private:
    bool m_callable = false;

    LOValueType m_base_type;
    std::shared_ptr<LOClass> m_concrete;
    std::string m_concrete_tmp;

    static std::shared_ptr<OliveType> typeVoid;
    static std::shared_ptr<OliveType> typeBoolean;
};

namespace AST {
class NodeTypeSpec;
}

std::shared_ptr<OliveType> convertTypeSpecNodeToOliveType(
        const AST::NodeTypeSpec& type, LOScript &script);
std::string convertOliveTypeToString(const OliveType& type);

enum LOValueOtherType : uint8_t {
    PrimitiveBoolean = 0x1 << 0,
    PrimitiveInt32 = 0x1 << 1,
    PrimitiveUInt32 = 0x1 << 2,
    PrimitiveInt8 = 0x1 << 3,
    PrimitiveUndefined = 0x1 << 4,
    PrimitiveNull = 0x1 << 5
};

enum LOValueOtherTypeComposite : uint8_t {
    CompositeInteger = PrimitiveInt32 | PrimitiveUInt32 | PrimitiveInt8,
    CompositeInt32 = PrimitiveInt32 | PrimitiveUInt32,
    CompositeSignedInt = PrimitiveInt8 | PrimitiveInt32
};

// num_tag == 0xffff -> (u)int32, boolean, undefined
// 0 < num_tag < 0xffff -> double
// num_tag == 0x0000 -> cell
const uint64_t TAG_MISC_PRIMITIVE   =   0xffff000000000000;
const uint64_t DOUBLE_OFFSET        =   0x0001000000000000;

typedef uint64_t OliveCode;

union LOValueLayout {
    uint64_t as_u64; // #64

    struct tagging { // #64
        // #32
        union {
            struct { // #32
                uint16_t num_tag : 16;
                LOValueOtherType primitive : 8;
                int8_t placeholder : 8;
            };
            uint32_t tag;
        };
        union { // #32
            // signed int8
            struct {
                uint32_t placeholder : 24;
                int8_t i8 : 8;
            } as_i8;
            // bool
            struct {
                uint32_t placeholder: 31;
                bool b : 1;
            } as_bool;
            // signed/unsigned int32
            union {
                uint32_t ui;
                int32_t i;
            } as_int32;
        };
    } tags;

    void *as_ptr; // #64
};

struct LOValue {
public:

    OliveCode code() const {
        return (OliveCode) encoded.as_u64; }

    bool isBoolean() const {
        return isMiscPrimitive() && encoded.tags.primitive == PrimitiveBoolean; }

    bool isNumber() const {
        return (isDouble() || isSomeKindOfInteger()); }

    bool isDouble() const {
        return ((encoded.tags.num_tag != 0) &&
                !isMiscPrimitive()); }

    bool isMiscPrimitive() const {
        return (encoded.tags.num_tag == 0xffff); }

    bool isSomeKindOfInteger() const {
        return (isMiscPrimitive() && (encoded.tags.primitive & CompositeInteger)); }

    bool isInt32OrUInt32() const {
        return (isMiscPrimitive() && (encoded.tags.primitive & CompositeInt32)); }

    bool isInt32() const {
        return (isMiscPrimitive() && encoded.tags.primitive == PrimitiveInt32); }

    bool isUInt32() const {
        return (isMiscPrimitive() && encoded.tags.primitive == PrimitiveUInt32); }

    bool isInt8() const {
        return (isMiscPrimitive() && encoded.tags.primitive == PrimitiveInt8); }

    bool isCell() const {
        return (encoded.tags.num_tag == 0); }

    bool toBooleanUnsafe() const {
        return (encoded.tags.as_bool.b); }

    double toDoubleUnsafe() const {
        union {
            uint64_t i; double d;
        } ret;
        ret.i = encoded.as_u64;
        ret.i -= DOUBLE_OFFSET;
        return ret.d;
    }

    int8_t toInt8Unsafe() const {
        return encoded.tags.as_i8.i8; }

    int32_t toInt32Unsafe() const {
        return encoded.tags.as_int32.i; }

    uint32_t toUInt32Unsafe() const {
        return encoded.tags.as_int32.ui; }

    void *toPointerUnsafe() const {
        return encoded.as_ptr; }

    void setDouble(double val) {
        union {
            uint64_t i; double d;
        } ret;
        ret.d = val;
        encoded.as_u64 = ret.i + DOUBLE_OFFSET;
    }

    void setInt32(int32_t val) {
        encoded.as_u64 = TAG_MISC_PRIMITIVE;
        encoded.tags.primitive = PrimitiveInt32;
        encoded.tags.as_int32.i = val;
    }

    void setUInt32(uint32_t val) {
        encoded.as_u64 = TAG_MISC_PRIMITIVE;
        encoded.tags.primitive = PrimitiveUInt32;
        encoded.tags.as_int32.ui = val;
    }

    void setInt8(int8_t val) {
        encoded.as_u64 = TAG_MISC_PRIMITIVE;
        encoded.tags.primitive = PrimitiveInt8;
        encoded.tags.as_i8.i8 = val;
    }

    void setBoolean(bool value) {
        encoded.as_u64 = TAG_MISC_PRIMITIVE;
        encoded.tags.primitive = PrimitiveBoolean;
        encoded.tags.as_bool.b = value;
    }

    void setPointer(void *ptr) {
        assert(((uint64_t) ptr & TAG_MISC_PRIMITIVE) == 0);
        encoded.as_ptr = ptr;
    }

private:
    LOValueLayout encoded;
};

}

#endif // VMFOROLIVE_LOVALUE_HXX
