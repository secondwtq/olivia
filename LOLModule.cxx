//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOLModule.hxx"
#include "LOScript.hxx"

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include <memory>

namespace Olivia {

LOLModule::LOLModule(const char *name, std::shared_ptr<LOScript> script)
    : m_script(script),
      m_lmodule(std::make_shared<llvm::Module>(name, llvm::getGlobalContext())),
        m_builder(std::make_shared<llvm::IRBuilder<>>(llvm::getGlobalContext())) {

}

llvm::Function *LOLModule::getGlobalFunctionCodeNamed(const std::string& name) {
    if (auto *f = module()->getFunction(name)) {
        return f; }

    auto fi = script()->getGlobalFunctionNamed(name);
    if (fi) {
        return reinterpret_cast<llvm::Function *>(fi->generate_code(this)); }
    return nullptr;
}

llvm::AllocaInst *LOLModule::createAllocaAtEntry(llvm::Function *func, const std::string& name, llvm::Type *tp) {
    llvm::IRBuilder<> b(&func->getEntryBlock(), func->getEntryBlock().begin());
    return b.CreateAlloca(tp, 0, name);
}

}
