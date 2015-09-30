//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/26.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOHLBuilder.hxx"
#include "LOHLBasic.hxx"

namespace Olivia {
namespace HL {

std::shared_ptr<HLSemanticBlock> HLBlockBuilder::currentSemanticBlock() {
    if (currentBlock()) {
        return currentBlock()->semanticBlock(); }
    // TODO: multiple top-level semantic block
    return parentScript()->semanticBlock;
}

std::shared_ptr<HLIBranchJump> HLBlockBuilder::addBranchJump(std::shared_ptr<HLBlock> dest) {
    auto ret = std::make_shared<HLIBranchJump>();
    ret->dest = dest;
    currentBlock()->addInstruction(ret);
    return ret;
}

std::shared_ptr<HLIBranchConditionBinary> HLBlockBuilder::addBranchConditionBinary(
        std::shared_ptr<HLBlock> t, std::shared_ptr<HLBlock> f) {
    auto ret = std::make_shared<HLIBranchConditionBinary>();
    ret->dest_true = t, ret->dest_false = f;
    currentBlock()->addInstruction(ret);
    sstack->pop();
    return ret;
}

std::shared_ptr<HLIPop> HLBlockBuilder::addPop() {
    auto ret = std::make_shared<HLIPop>();
    currentBlock()->addInstruction(ret);
    sstack->pop();
    return ret;
}

std::shared_ptr<HLIPushConstantArithmetic> HLBlockBuilder::addPushConstantArithmeticBoolean(bool value) {
    auto ret = std::make_shared<HLIPushConstantArithmetic>();
    ret->m_type = std::make_shared<OliveType>(TypeBoolean);
    ret->bool_type = value;
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createConstantRef(ret->m_type));
    return ret;
}

std::shared_ptr<HLIPushConstantArithmetic> HLBlockBuilder::addPushConstantArithmeticInteger(int64_t value) {
    auto ret = std::make_shared<HLIPushConstantArithmetic>();
    ret->m_type = std::make_shared<OliveType>(TypeInt32);
    ret->int32_type = value;
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createConstantRef(ret->m_type));
    return ret;
}

std::shared_ptr<HLIPushConstantArithmetic> HLBlockBuilder::addPushConstantArithmeticDouble(double value) {
    auto ret = std::make_shared<HLIPushConstantArithmetic>();
    ret->m_type = std::make_shared<OliveType>(TypeDouble);
    ret->double_type = value;
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createConstantRef(ret->m_type));
    return ret;
}

std::shared_ptr<HLIPushLocalVariableRef> HLBlockBuilder::addPushLocalVariableRef(
        std::shared_ptr<OliveType> type, size_t idx) {
    auto ret = std::make_shared<HLIPushLocalVariableRef>();
    ret->m_type = type;
    ret->slotidx = idx;
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createLocalVariableRef(type, idx));
    return ret;
}

std::shared_ptr<HLIPushConstantGlobal> HLBlockBuilder::addPushConstantGlobal(
        std::shared_ptr<OliveType> type, const std::string& path) {
    auto ret = std::make_shared<HLIPushConstantGlobal>();
    ret->m_type = type;
    ret->path = path;
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createGlobalConstantRef(type));
    return ret;
}

std::shared_ptr<HLICallBase> HLBlockBuilder::addCall(size_t numargs) {
    std::shared_ptr<HLICallBase> ret = nullptr;
    auto func_type = sstack->indexed(numargs)->type;
    if (func_type->callReturnType->baseType() == TypeVoid) {
        ret = std::make_shared<HLICallVoid>();
    } else { ret = std::make_shared<HLICall>(); }
    ret->numberOfArguments = numargs;
    for (size_t i = 0; i < numargs+1; i++) {
        sstack->pop(); }
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createTemporary(func_type->callReturnType));
    return ret;
}

std::shared_ptr<HLIReturn> HLBlockBuilder::addReturn(std::shared_ptr<OliveType> type) {
    auto ret = std::make_shared<HLIReturn>();
    ret->m_type = currentSemanticBlock()->parentFunctionLocal()->returnType;
    sstack->pop();
    currentBlock()->addInstruction(ret);
    return ret;
}

std::shared_ptr<HLICast> HLBlockBuilder::addCast(std::shared_ptr<OliveType> from, std::shared_ptr<OliveType> to) {
    auto ret = std::make_shared<HLICast>();
    ret->from = from, ret->to = to;
    sstack->pop();
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createTemporary(to));
    return ret;
}

std::shared_ptr<HLIArithmeticBinaryAdd> HLBlockBuilder::addArithmeticBinaryAdd(std::shared_ptr<OliveType> type) {
    auto ret = std::make_shared<HLIArithmeticBinaryAdd>();
    ret->type = type;
    sstack->pop();
    sstack->pop();
    currentBlock()->addInstruction(ret);
    sstack->push(HLSimulatorStackValue::createTemporary(type));
    return ret;
}

}
}
