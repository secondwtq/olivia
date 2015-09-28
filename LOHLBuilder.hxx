//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/26.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOHLBUILDER_HXX
#define VMFOROLIVE_LOHLBUILDER_HXX

#include <deque>
#include <memory>
#include "LOValue.hxx"

namespace Olivia {
class OliveType;
namespace HL {
class HLScript;
class HLBlock;
class HLSemanticBlock;
}
}

namespace Olivia {
namespace HL {
class HLIBranchJump;
class HLIBranchConditionBinary;
class HLIPushConstantArithmetic;
class HLIPushLocalVariableRef;
class HLIPushConstantGlobal;
class HLIPop;
class HLICallBase;
class HLIReturn;
class HLICast;
}
}

namespace Olivia {
namespace HL {

template <typename T>
class HLSimulatorStack {
public:
    void push(const T& val) {
        queue.push_front(val); }
    T pop() {
        auto ret = queue.front();
        queue.pop_front();
        return ret;
    }
    T top() { return queue.front(); }
    T getIndexed(size_t idx) {
        return queue.at(idx); }
    std::deque<T> queue;
};

enum HLSimulatorStackValueKind {
    VNone,
    VLocal,
    VConstant,
    VGlobalConstant,
    VTemporary
};

class HLSimulatorStackValue {
public:

    HLSimulatorStackValue(HLSimulatorStackValueKind k)
            : kind(k) { }

    static std::shared_ptr<HLSimulatorStackValue> createLocalVariableRef(
            std::shared_ptr<OliveType> type, size_t idx) {
        auto ret = std::make_shared<HLSimulatorStackValue>(VLocal);
        ret->type = type, ret->idx = idx;
        return ret;
    }

    static std::shared_ptr<HLSimulatorStackValue> createConstantRef(
            std::shared_ptr<OliveType> type) {
        auto ret = std::make_shared<HLSimulatorStackValue>(VConstant);
        ret->type = type;
        return ret;
    }

    static std::shared_ptr<HLSimulatorStackValue> createGlobalConstantRef(
            std::shared_ptr<OliveType> type) {
        auto ret = std::make_shared<HLSimulatorStackValue>(VGlobalConstant);
        ret->type = type;
        return ret;
    }

    static std::shared_ptr<HLSimulatorStackValue> createTemporary(
            std::shared_ptr<OliveType> type) {
        auto ret = std::make_shared<HLSimulatorStackValue>(VTemporary);
        ret->type = type;
        return ret;
    }

    HLSimulatorStackValueKind kind = VNone;
    std::shared_ptr<OliveType> type = nullptr;
    size_t idx = 0;
};

class HLBlockBuilder {
public:

    HLBlockBuilder(std::shared_ptr<HLScript> script) : m_script(script), sstack
            (std::make_shared<HLSimulatorStack<std::shared_ptr<HLSimulatorStackValue>>>()) { }

    void setCurrentBlock(std::shared_ptr<HLBlock> block) {
        m_current_block = block; }
    std::shared_ptr<HLBlock> currentBlock() {
        return m_current_block; }
    std::shared_ptr<HLScript> parentScript() {
        return m_script.lock(); }

    std::shared_ptr<HLSemanticBlock> currentSemanticBlock();

    std::shared_ptr<HLIBranchJump> addBranchJump(std::shared_ptr<HLBlock> dest);
    std::shared_ptr<HLIBranchConditionBinary> addBranchConditionBinary(
            std::shared_ptr<HLBlock> t, std::shared_ptr<HLBlock> f);

    // this stack operation helpers are decorated with sstack simulations.
    std::shared_ptr<HLIPushConstantArithmetic> addPushConstantArithmeticBoolean(bool value);
    std::shared_ptr<HLIPushConstantArithmetic> addPushConstantArithmeticInteger(int64_t value);
    std::shared_ptr<HLIPushConstantArithmetic> addPushConstantArithmeticDouble(double value);
    std::shared_ptr<HLIPushLocalVariableRef> addPushLocalVariableRef(
            std::shared_ptr<OliveType> type, size_t idx);
    std::shared_ptr<HLIPushConstantGlobal> addPushConstantGlobal(
            std::shared_ptr<OliveType> type, const std::string& path);

    std::shared_ptr<HLICallBase> addCall(size_t numargs);
    std::shared_ptr<HLIReturn> addReturn(
            std::shared_ptr<OliveType> type);

    std::shared_ptr<HLIPop> addPop();
    std::shared_ptr<HLICast> addCast(std::shared_ptr<OliveType> from, std::shared_ptr<OliveType> to);

    std::shared_ptr<HLSimulatorStack<std::shared_ptr<HLSimulatorStackValue>>> sstack;

private:
    std::shared_ptr<HLBlock> m_current_block;
    std::weak_ptr<HLScript> m_script;
};

}
}

#endif // VMFOROLIVE_LOHLBUILDER_HXX
