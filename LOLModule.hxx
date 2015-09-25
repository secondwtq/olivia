//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOLMODULE_HXX
#define VMFOROLIVE_LOLMODULE_HXX

#include "LOValue.hxx"

#include <string>
#include <memory>
#include <unordered_map>
#include <stack>

#include <llvm/IR/IRBuilder.h>

namespace llvm {
class Module;
}

namespace Olivia {

class LOScript;

struct LOLLocalDef {
public:
    LOLLocalDef(std::shared_ptr<OliveType> t, llvm::Value *v)
            : type(t), value(v) { }
    std::shared_ptr<OliveType> type;
    llvm::Value *value;
};

class LOLGenBlock {
public:

    LOLGenBlock() : LOLGenBlock(nullptr) { }
    LOLGenBlock(llvm::BasicBlock *bb)
            : block(bb) { }

    llvm::BasicBlock *block;
    std::unordered_map<std::string, std::shared_ptr<LOLLocalDef>> locals;

    bool hasLocalNamed(const std::string& name) const {
        auto t = locals.find(name);
        if (t == locals.end()) {
            return false;
        } else { return true; }
    }

    std::shared_ptr<LOLLocalDef> getLocalNamed(const std::string& name) {
        auto t = locals.find(name);
        if (t == locals.end()) {
            return nullptr;
        } else { return t->second; }
    }
};

class LOLModule {
public:

    LOLModule(const char *name, std::shared_ptr<LOScript> script);

    inline std::shared_ptr<llvm::Module> module() {
        return m_lmodule; }

    inline std::shared_ptr<llvm::IRBuilder<>> builder() {
        return m_builder; }

    inline std::shared_ptr<LOScript> script() {
        return m_script; }

    llvm::Function *getGlobalFunctionCodeNamed(const std::string& name);
    static llvm::AllocaInst *createAllocaAtEntry(
            llvm::Function *func, const std::string& name, llvm::Type *tp);

    std::shared_ptr<LOLGenBlock> blockTop() {
        return m_blocks.top(); }

    void blockPush(std::shared_ptr<LOLGenBlock> block) {
        m_blocks.push(block); }

    void blockPop() {
        m_blocks.pop(); }

private:
    std::stack<std::shared_ptr<LOLGenBlock>> m_blocks;

    std::shared_ptr<LOScript> m_script;
    std::shared_ptr<llvm::Module> m_lmodule;
    std::shared_ptr<llvm::IRBuilder<>> m_builder;
};

}

#endif // VMFOROLIVE_LOLMODULE_HXX
