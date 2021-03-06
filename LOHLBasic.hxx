//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOHLBASIC_HXX
#define VMFOROLIVE_LOHLBASIC_HXX

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include <unordered_map>

#include "LOValue.hxx"

#include "LOIntrusiveList.hxx"

namespace Olivia {
namespace HL {

enum HLInstrKind {
    HUnknown,
    HPushLocalVariableRef,
    HPushGlobalVariableRef,
    HPushArgumentRef,
    HPushConstantArithmetic,
    HPushConstantGlobal,
    HAllocLocalVariable,
    HFreeLocalVariable,
    HStoreLocalVariable,
    HPop,
    HCallVoid,
    HCall,
    HCast,
    HReturn,
    HBranchJump,
    HBranchConditionBinary,

    HArithmeticBinaryAdd,
    HArithmeticBinaryMul,
    HArithmeticBinaryDiv,
    HArithmeticBinaryBitAnd,
    HArithmeticBinaryBitOr,
    HArithmeticBinaryAnd,
    HArithmeticBinaryOr,
    HArithmeticUnaryNegative,
    HArithmeticBinaryCompare
};

class HLIBase : public Base::IntrusiveListNode<HLIBase,
        std::shared_ptr<HLIBase>, std::shared_ptr<const HLIBase>> {
public:

    virtual HLInstrKind kind() const = 0;
    virtual std::string instructionName() const = 0;
    virtual std::string toString() const = 0;
    virtual ~HLIBase() { }

};

class HLBlock;
class HLSemanticBlock;
class HLFunction;
class HLScript;

struct HLVariableInfo {
    std::string name;
    std::shared_ptr<OliveType> type;
    virtual bool isLocal() const { return false; }
    virtual bool isConstant() const { return false; }
    virtual ~HLVariableInfo() { }
};

struct HLVariableInfoLocal : public HLVariableInfo {
    bool isLocal() const override { return true; }
    size_t slotID;
};

struct HLVariableInfoGlobalConstant : public HLVariableInfo {
    bool isConstant() const override { return true; }
};

class HLSemanticBlock : public std::enable_shared_from_this<HLSemanticBlock> {
public:
    HLSemanticBlock(std::shared_ptr<HLSemanticBlock> block)
            : parent(block) { }
    virtual ~HLSemanticBlock() { }

    std::weak_ptr<HLSemanticBlock> parent;
    std::vector<std::shared_ptr<HLSemanticBlock>> childBlocks;
    std::unordered_map<std::string, std::shared_ptr<HLVariableInfoLocal>> localLookupTable;
    std::unordered_map<std::string, std::shared_ptr<HLVariableInfo>> staticLookupTable;

    bool hasOwnName(const std::string& name);
    std::shared_ptr<HLVariableInfo> resolveVariableNamed(const std::string& name);
    size_t allocNameLocal(const std::string& name, std::shared_ptr<OliveType> type);
    std::shared_ptr<HLVariableInfo> allocNameStatic(const std::string& name,
            std::shared_ptr<OliveType> type, bool isConstant = false);

    virtual bool isFunction() const { return false; }
    std::shared_ptr<HLBlock> openBlock();
    std::shared_ptr<HLSemanticBlock> appendSemanticBlock();
    // the behavior of these two functions below differs
    //  when the SemanticBlock is a Function itself.
    std::shared_ptr<HLFunction> parentFunctionClosured();
    std::shared_ptr<HLFunction> parentFunctionLocal();
};

class HLBlock {
public:

    void addInstruction(std::shared_ptr<HLIBase> instr) {
        m_instructions.push_back(instr); }

    std::shared_ptr<HLSemanticBlock> semanticBlock() {
        return m_semantic_block.lock(); }
    void setSemanticBlock(std::shared_ptr<HLSemanticBlock> block) {
        m_semantic_block = block; }
    std::string toString() const;

    std::shared_ptr<HLIBase> topInstruction() {
        return m_instructions.tail(); }

    void insertInstruction(std::shared_ptr<HLIBase> pos,
            std::shared_ptr<HLIBase> ptr) {
        m_instructions.insert(pos, ptr); }

    int64_t tmpID;

private:
    std::weak_ptr<HLSemanticBlock> m_semantic_block;
    Base::IntrusiveList<HLIBase> m_instructions;
};

class HLFunction : public HLSemanticBlock {
public:
    static std::shared_ptr<HLFunction> create(std::shared_ptr<HLScript> script,
            std::shared_ptr<HLSemanticBlock> parent_block,
            const std::vector<std::shared_ptr<HLVariableInfo>>& args);

    bool isFunction() const override { return true; }
    std::shared_ptr<HLScript> parentScript() {
        return m_parent.lock(); }
    size_t allocLocalSlot() {
        return m_local_idx_max++; }
    std::string toString() const;

    std::shared_ptr<OliveType> returnType;
    std::vector<std::shared_ptr<HLVariableInfo>> arguments;
    std::vector<std::shared_ptr<HLBlock>> blocks;
    std::shared_ptr<HLBlock> entryBlock;

private:
    struct shared_exclusive { };
public:
    HLFunction(std::shared_ptr<HLScript> script,
            std::shared_ptr<HLSemanticBlock> parent_block,
            const std::vector<std::shared_ptr<HLVariableInfo>>& args, shared_exclusive);
private:
    void initialize();

    std::weak_ptr<HLScript> m_parent;
    size_t m_local_idx_max = 0;
};

class HLScript {
public:

    HLScript(std::shared_ptr<LOScript> script);
    std::string toString() const;

    std::shared_ptr<HLSemanticBlock> semanticBlock;
    std::vector<std::shared_ptr<HLFunction>> functions;
    std::shared_ptr<LOScript> lscript;
};

class HLIPush : public HLIBase { };

class HLIPushLocalVariableRef : public HLIPush {
public:
    HLInstrKind kind() const override {
        return HPushLocalVariableRef; }
    std::string instructionName() const override {
        return "push localvar"; }
    std::string toString() const override {
        return instructionName() + " (" + convertOliveTypeToString(*type()) +
                ") " + std::to_string(slotidx); }
    size_t slotidx;
    std::shared_ptr<OliveType> type() const {
            return m_type; }
    std::shared_ptr<OliveType> m_type;
};

class HLIPushGlobalVariableRef : public HLIPush {
public:
    HLInstrKind kind() const override {
        return HPushGlobalVariableRef; }
    std::string instructionName() const override {
        return "push global"; }
    std::string toString() const override {
        return instructionName() + " (" + convertOliveTypeToString(*type()) +
                                         ") "; }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;
};

class HLIPushConstant : public HLIPush { };

class HLIPushConstantArithmetic : public HLIPush {
public:
    HLInstrKind kind() const override {
        return HPushConstantArithmetic; }
    std::string instructionName() const override {
        return "push constant"; }
    std::string toString() const override {
        std::string ret = instructionName() + " (" + convertOliveTypeToString(*type()) + ") ";
        if (type()->baseType() == TypeBoolean) {
            ret += std::to_string(bool_type);
        } else if (type()->baseType() == TypeInt32) {
            ret += std::to_string(int32_type);
        } else if (type()->baseType() == TypeDouble) {
            ret += std::to_string(double_type); }
        return ret;
    }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;

    union {
        bool bool_type;
        int32_t int32_type;
        double double_type;
    };
};

class HLIPushConstantGlobal : public HLIPush {
public:
    HLInstrKind kind() const override {
        return HPushConstantGlobal; }
    std::string instructionName() const override {
        return "push globalconstant"; }
    std::string toString() const override {
        std::string ret = instructionName() + " (" +
                convertOliveTypeToString(*type()) + ") " + path;
        return ret;
    }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;
    std::string path;
};

//
// push constant int32 4
// store localvar 2
//
class HLIStoreLocalVariable : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HStoreLocalVariable; }
    std::string instructionName() const override {
        return "store localvar"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type()) +
                " " + std::to_string(slotidx);
    }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;
    size_t slotidx;
};

class HLIAllocLocalVariable : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HAllocLocalVariable; }
    std::string instructionName() const override {
        return "localvar alloc"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type()) +
                                         " " + std::to_string(slotidx);
    }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;
    size_t slotidx;
};

class HLIFreeLocalVariable : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HFreeLocalVariable; }
    std::string instructionName() const override {
        return "localvar free"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type()) +
               " " + std::to_string(slotidx);
    }
    std::shared_ptr<OliveType> type() const {
        return m_type; }
    std::shared_ptr<OliveType> m_type;
    size_t slotidx;
};

class HLIArithmeticCast : public HLIBase { };

class HLIArithmetic : public HLIBase { };

class HLIPop : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HPop; }
    std::string instructionName() const override {
        return "pop"; }
    std::string toString() const override {
        return instructionName(); }
};

class HLICallBase : public HLIBase {
public:
    size_t numberOfArguments;
};

//
// push global function printf
// push constant rawstring "%d\n"
// push local 3
// callvoid 2
//
class HLICallVoid : public HLICallBase {
public:
    HLInstrKind kind() const override {
        return HCallVoid; }
    std::string instructionName() const override {
        return "callvoid"; }
    std::string toString() const override {
        return instructionName() + " " + std::to_string(numberOfArguments); }
};

//
// push global function add_int32
// push constant int32 1
// push constant int32 2
// call 2
// (probably) pop;
//
class HLICall : public HLICallBase {
public:
    HLInstrKind kind() const override {
        return HCall; }
    std::string instructionName() const override {
        return "call"; }
    std::string toString() const override {
        return instructionName() + " " + std::to_string(numberOfArguments); }
};

class HLICast : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HCast; }
    std::string instructionName() const override {
        return "cast"; }
    std::string toString() const override {
        return instructionName() + " " +
                convertOliveTypeToString(*from) + " " + convertOliveTypeToString(*to);
    }
    std::shared_ptr<OliveType> from;
    std::shared_ptr<OliveType> to;
};

class HLIReturn : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HReturn; }
    std::string instructionName() const override {
        return "return"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type()); }
    std::shared_ptr<OliveType> type() const {
        return m_type; }

    std::shared_ptr<OliveType> m_type;
};

class HLIBranch : public HLIBase { };

class HLIBranchJump : public HLIBranch {
public:
    HLInstrKind kind() const override {
        return HBranchJump; }
    std::string instructionName() const override {
        return "branch jump"; }
    std::string toString() const override {
        return instructionName() + " " + std::to_string(dest->tmpID); }
    std::shared_ptr<HLBlock> dest;
};

class HLIBranchConditionBinary : public HLIBranch {
public:
    HLInstrKind kind() const override {
        return HBranchConditionBinary; }
    std::string instructionName() const override {
            return "branch bicondition"; }
    std::string toString() const override {
            return instructionName() + " " + std::to_string(dest_true->tmpID) +
                    " " + std::to_string(dest_false->tmpID); }

    std::shared_ptr<HLBlock> dest_true;
    std::shared_ptr<HLBlock> dest_false;
};

class HLIArithmeticBinaryAdd : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryAdd; }
    std::string instructionName() const override {
        return "add"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type); }

    std::shared_ptr<OliveType> type;
};

class HLIArithmeticBinaryMul : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryMul; }
    std::string instructionName() const override {
        return "mul"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type); }

    std::shared_ptr<OliveType> type;
};

class HLIArithmeticBinaryDiv : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryDiv; }
    std::string instructionName() const override {
        return "div"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type); }

    std::shared_ptr<OliveType> type;
};

class HLIArithmeticBinaryBitAnd : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryBitAnd; }
    std::string instructionName() const override {
        return "bitand"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type); }

    std::shared_ptr<OliveType> type;
};

class HLIArithmeticBinaryBitOr : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryBitOr; }
    std::string instructionName() const override {
        return "bitor"; }
    std::string toString() const override {
        return instructionName() + " " + convertOliveTypeToString(*type); }

    std::shared_ptr<OliveType> type;
};

class HLIArithmeticBinaryAnd : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryAnd; }
    std::string instructionName() const override {
        return "and"; }
    std::string toString() const override {
        return instructionName(); }
};

class HLIArithmeticBinaryOr : public HLIBase {
public:
    HLInstrKind kind() const override {
        return HArithmeticBinaryOr; }
    std::string instructionName() const override {
        return "or"; }
    std::string toString() const override {
        return instructionName(); }
};

class HLIArithmeticUnaryNegative : public HLIBase {
public:
    HLInstrKind kind() const override { return HArithmeticUnaryNegative; }
    std::string instructionName() const override { return "neg"; }
    std::string toString() const override { return instructionName() + ' ' + convertOliveTypeToString(*type); }
    std::shared_ptr<OliveType> type;
};

enum ArithmeticCompareType {
    CEqual, CNotEqual, CGreater, CLess,
    CGreaterEqual, CLessEqual
};

const char * convertCompareTypeToString(ArithmeticCompareType t);

class HLIArithmeticBinaryCompare : public HLIBase {
public:
    HLInstrKind kind() const override { return HArithmeticBinaryCompare; }
    std::string instructionName() const override { return "compare"; }
    std::string toString() const override { return instructionName() + ' ' + convertOliveTypeToString(*type) +
                ' ' + convertCompareTypeToString(ctype); }
    std::shared_ptr<OliveType> type;
    ArithmeticCompareType ctype;
};

}
}

#endif // VMFOROLIVE_LOHLBASIC_HXX
