//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOHLBasic.hxx"

#include <assert.h>

namespace Olivia {
namespace HL {

std::shared_ptr<HLSemanticBlock> HLBlockBuilder::currentSemanticBlock() {
    if (currentBlock()) {
        return currentBlock()->semanticBlock(); }
    // TODO: multiple top-level semantic block
    return parentScript()->semanticBlock;
}

HLFunction::HLFunction(std::shared_ptr<HLScript> parent, std::shared_ptr<HLSemanticBlock> parent_block,
        const std::vector<std::shared_ptr<HLVariableInfo>>& args, shared_exclusive)
        : HLSemanticBlock(parent_block), m_parent(parent) { }

std::shared_ptr<HLFunction> HLFunction::create(std::shared_ptr<HLScript> script,
        std::shared_ptr<HLSemanticBlock> parent_block,
        const std::vector<std::shared_ptr<HLVariableInfo>>& args) {
    auto ret = std::make_shared<HLFunction>(script, parent_block,
            args, shared_exclusive());

    ret->entryBlock = std::make_shared<HLBlock>();
    ret->blocks.push_back(ret->entryBlock);
    ret->entryBlock->setSemanticBlock(ret);

    for (auto arg : args) {
        ret->arguments.push_back(arg);
        ret->allocNameLocal(arg->name, arg->type);
    }
    return ret;
}

std::shared_ptr<HLVariableInfo> HLSemanticBlock::resolveVariableNamed(
        const std::string& name) {
    auto i = localLookupTable.find(name);
    if (i != localLookupTable.end()) {
        return i->second; }
    auto j = staticLookupTable.find(name);
    if (j != staticLookupTable.end()) {
        return j->second; }
    if (!parent.expired()) {
        return parent.lock()->resolveVariableNamed(name); }
    return nullptr;
}

bool HLSemanticBlock::hasOwnName(const std::string& name) {
    auto i = localLookupTable.find(name);
    if (i != localLookupTable.end()) {
        return true; }
    auto j = staticLookupTable.find(name);
    if (j != staticLookupTable.end()) {
        return true; }
    return false;
}

size_t HLSemanticBlock::allocNameLocal(const std::string& name, std::shared_ptr<OliveType> type) {
    assert(!hasOwnName(name));
    auto ret = std::make_shared<HLVariableInfoLocal>();
    ret->name = name, ret->type = type;
    ret->slotID = parentFunctionLocal()->allocLocalSlot();
    localLookupTable.insert({ name, ret });
    return ret->slotID;
}

std::shared_ptr<HLVariableInfo> HLSemanticBlock::allocNameStatic(
        const std::string& name, std::shared_ptr<OliveType> type) {
    assert(!hasOwnName(name));
    auto ret = std::make_shared<HLVariableInfo>();
    ret->name = name, ret->type = type;
    staticLookupTable.insert({ name, ret });
    return ret;
}

std::shared_ptr<HLBlock> HLSemanticBlock::openBlock() {
    auto ret = std::make_shared<HLBlock>();
    ret->setSemanticBlock(this->shared_from_this());
    parentFunctionLocal()->blocks.push_back(ret);
    return ret;
}

std::shared_ptr<HLSemanticBlock> HLSemanticBlock::appendSemanticBlock() {
    auto semantic_block = std::make_shared<HLSemanticBlock>(shared_from_this());
    childBlocks.push_back(semantic_block);
    return semantic_block;
}

std::shared_ptr<HLFunction> HLSemanticBlock::parentFunctionClosured() {
    if (parent.expired()) {
        return nullptr; }
    std::shared_ptr<HLSemanticBlock> p = parent.lock();
    if (p->isFunction()) {
        return std::static_pointer_cast<HLFunction>(p); }
    return p->parentFunctionClosured();
}

std::shared_ptr<HLFunction> HLSemanticBlock::parentFunctionLocal() {
    if (isFunction()) {
        return std::static_pointer_cast<HLFunction>(shared_from_this()); }
    if (parent.expired()) {
        return nullptr; }
    return parent.lock()->parentFunctionLocal();
}

HLScript::HLScript(std::shared_ptr<LOScript> script) :
        semanticBlock(std::make_shared<HLSemanticBlock>(nullptr)), lscript(script) { }

std::string HLBlock::toString() const {
    std::string ret;
    for (auto instruction : m_instructions) {
        ret += instruction->toString();
        ret += "\n";
    }
    return ret;
}

std::string HLFunction::toString() const {
    std::string ret;
    for (auto block : blocks) {
        ret += block->toString();
    }
    return ret;
}

std::string HLScript::toString() const {
    std::string ret;
    for (auto function : functions) {
        ret += function->toString(); }
    return ret;
}

}
}
