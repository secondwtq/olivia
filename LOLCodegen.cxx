//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOAST.hxx"
#include "LOLCodegen.hxx"
#include "LOLModule.hxx"
#include "LOScript.hxx"

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>

#include <vector>

namespace Olivia {

using namespace llvm;

Type *convertValueTypeToLLVMType(LOValueType type) {
    switch (type) {
        case TypeVoid:
            return Type::getVoidTy(llvm::getGlobalContext());
        case TypeInt8:
            return Type::getInt8Ty(llvm::getGlobalContext());
        case TypeInt32:
            return Type::getInt32Ty(llvm::getGlobalContext());
        case TypeUInt32:
            return Type::getInt32Ty(llvm::getGlobalContext());
        case TypeDouble:
            return Type::getDoubleTy(llvm::getGlobalContext());
    }
    return nullptr;
}

Type *convertTypeToLLVMType(const OliveType& type) {
    return convertValueTypeToLLVMType(type.baseType());
}

Type *convertTypeSpecNodeToLLVMType(const AST::NodeTypeSpec& type) {
    return convertValueTypeToLLVMType(type.base_type);
}

OliveType convertTypeSpecNodeToOliveType(const AST::NodeTypeSpec& type, LOScript *script) {
    OliveType ret;
    ret.setBaseType(type.base_type);
    if (type.concrete_type) {
        ret.setConcreteName(type.concrete_type->name); }
    ret.findConcrete(script);
    return ret;
}

namespace AST {

llvm::Value *NodeConstantBoolean::generate_code(LOLModule *module) {
    if (value) {
        return module->builder()->getTrue();
    } else { return module->builder()->getFalse(); }
}

llvm::Value *NodeConstantDouble::generate_code(LOLModule *module) {
    return ConstantFP::get(llvm::getGlobalContext(), APFloat(value)); }

llvm::Value *NodeConstantInteger::generate_code(LOLModule *module) {
    return ConstantInt::get(module->builder()->getInt32Ty(), value, true); }

// TODO: finish up <String>.
llvm::Value *NodeConstantString::generate_code(LOLModule *module) {
    return module->builder()->CreateGlobalString({ value }); }

llvm::Value *NodeBlock::generate_code(LOLModule *module) {
    for (auto statement : statements) {
        statement->generate_code(module); }
}

llvm::Value *NodeDeclarationFunction::generate_code(LOLModule *module) {
    Function *func = dyn_cast<Function>(signature->generate_code(module));
    BasicBlock *bb = BasicBlock::Create(llvm::getGlobalContext(), "entry", func);

    std::shared_ptr<LOLGenBlock> lb = std::make_shared<LOLGenBlock>();
    lb->block = bb;

    module->blockPush(lb);
    module->builder()->SetInsertPoint(bb);

    llvm::Value *b = body->generate_code(module);
    // if (llvm::Value *b = body->generate_code(module)) {
        verifyFunction(*func);
        module->blockPop();

        return func;
    // }
//    func->eraseFromParent();
//    return nullptr;
}

llvm::Value *NodeDeclarationExternFunction::generate_code(LOLModule *module) {
    return signature->generate_code(module); }

llvm::Value *NodeDeclarationSignature::generate_code(LOLModule *module) {
    std::vector<Type *> arg_types;
    for (auto arg : parameters) {
        arg_types.push_back(convertTypeSpecNodeToLLVMType(*arg->type)); }
    FunctionType *ft = FunctionType::get(convertTypeSpecNodeToLLVMType(*return_type), arg_types, false);
    Function *f = Function::Create(ft, Function::ExternalLinkage,
            this->trusted_name, module->module().get());

    size_t idx = 0;
    for (auto &arg : f->args()) {
        arg.setName(parameters[idx]->name->name);
        idx++;
    }

    return f;
}

llvm::Value *NodeCallExpression::generate_code(LOLModule *module) {
    Function *callee_func = llvm::dyn_cast<Function>(callee->generate_code(module));
    assert(callee_func);

    std::vector<Value *> argv;
    for (auto arg : arguments) {
        argv.push_back(arg->generate_code(module)); }

    // well if you assign some name to it
    //  then it would fail on <void>.
    return module->builder()->CreateCall(callee_func, argv);
}

llvm::Value *NodeIdentifier::generate_code(LOLModule *module) {
    ResolveResult resolved = module->script()->resolveIdentifier(this);
    switch (resolved.type) {
        case RGlobalFunction:
            return module->getGlobalFunctionCodeNamed(name);
        case RNone:
        {
            if (module->blockTop()->hasLocalNamed(name)) {
                return module->builder()->CreateLoad(
                        module->blockTop()->getLocalNamed(name)->value, name);
            }
        }
    }

    return nullptr;
}

llvm::Value *NodeStatementExpression::generate_code(LOLModule *module) {
    return expression->generate_code(module); }

llvm::Value *NodeStatementVar::generate_code(LOLModule *module) {
    return list->generate_code(module); }

llvm::Value *NodeDeclarationVar::generate_code(LOLModule *module) {
    assert(!module->blockTop()->hasLocalNamed(name->name));

    AllocaInst *alloca = LOLModule::createAllocaAtEntry(
            module->builder()->GetInsertBlock()->getParent(),
            name->name, convertTypeSpecNodeToLLVMType(*type));
    module->blockTop()->locals[name->name] = std::make_shared<LOLLocalDef>(
            convertTypeSpecNodeToOliveType(*type, module->script().get()), alloca);

    return alloca;
}

llvm::Value *MiscVarDeclarationList::generate_code(LOLModule *module) {
    for (auto decl : vars) {
        decl->generate_code(module); }
    return nullptr;
}

llvm::Value *NodeStatementReturn::generate_code(LOLModule *module) {
    module->builder()->CreateRet(expression->generate_code(module));
}

llvm::Value *NodeBinaryExpression::generate_code(LOLModule *module) {

}

}
}
