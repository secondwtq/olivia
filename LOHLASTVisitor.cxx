//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOHLASTVisitor.hxx"
#include "LOAST.hxx"
#include "LOType.hxx"

namespace Olivia {
namespace HL {

using namespace AST;

void HLEmitterVisitor::visit(NodeDeclarationExternFunction *node) {
    builder()->currentSemanticBlock()->allocNameStatic(
            node->name->name, convertSignatureToOliveType(
            *node->signature, *builder()->parentScript()->lscript));
}

void HLEmitterVisitor::visit(AST::NodeDeclarationFunction *node) {
    builder()->currentSemanticBlock()->allocNameStatic(
            node->name->name, convertSignatureToOliveType(
            *node->signature, *builder()->parentScript()->lscript));

    std::vector<std::shared_ptr<HLVariableInfo>> args;
    for (auto arg : node->signature->parameters) {
        auto arg_info = std::make_shared<HLVariableInfo>();
        arg_info->name = arg->name->name;
        arg_info->type = convertTypeSpecNodeToOliveType(*arg->ptype,
                *builder()->parentScript()->lscript);
        args.push_back(arg_info);
    }

    auto func = HLFunction::create(builder()->parentScript(),
            builder()->currentSemanticBlock(), args);
    // TODO: add to factory function
    func->returnType = convertTypeSpecNodeToOliveType(
            *node->signature->return_type, *builder()->parentScript()->lscript);
    builder()->parentScript()->functions.push_back(func);
    builder()->setCurrentBlock(func->entryBlock);

    for (auto statement : node->body->statements) {
        statement->accept(this); }

    // TODO: in-order free of local variables
    // TODO: you are 'freeing' variables after the 'return' instruction
    //      you should do it before it returns. so it remains a T0D0, disabled.
//    for (auto local : builder()->currentSemanticBlock()->localLookupTable) {
//        auto instr = std::make_shared<HLIFreeLocalVariable>();
//        instr->slotidx = local.second->slotID;
//        instr->m_type = local.second->type;
//        builder()->currentBlock()->addInstruction(instr);
//    }

    builder()->setCurrentBlock(nullptr);
}

void HLEmitterVisitor::visit(AST::NodeStatementExpression *node) {
    // pop
    node->expression->accept(this);
    auto instr = std::make_shared<HLIPop>();
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeStatementVar *node) {
    node->list->accept(this); }

void HLEmitterVisitor::visit(AST::MiscVarDeclarationList *node) {
    for (auto var : node->vars) {
        var->accept(this); }
}

void HLEmitterVisitor::visit(AST::NodeDeclarationVar *node) {
    auto type = convertTypeSpecNodeToOliveType(*node->type,
            *builder()->parentScript()->lscript);
    size_t idx = builder()->currentSemanticBlock()->allocNameLocal(
            node->name->name, type);

    // localvar alloc <type> <idx>
    auto instr = std::make_shared<HLIAllocLocalVariable>();
    instr->m_type = type;
    instr->slotidx = idx;
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeBlock *node) {
    auto semantic_block = builder()->currentSemanticBlock()->appendSemanticBlock();
    auto block = semantic_block->openBlock();
    builder()->setCurrentBlock(block);

    for (auto statement : node->statements) {
        statement->accept(this); }
}

void HLEmitterVisitor::visit(AST::NodeConstantBoolean *node) {
    // push constant boolean <value>
    auto instr = std::make_shared<HLIPushConstantArithmetic>();
    instr->m_type = std::make_shared<OliveType>(TypeBoolean);
    instr->bool_type = node->value;
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeConstantInteger *node) {
    // push constant int32 <value>
    auto instr = std::make_shared<HLIPushConstantArithmetic>();
    instr->m_type = std::make_shared<OliveType>(TypeInt32);
    instr->int32_type = node->value;
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeConstantDouble *node) {
    // push constant double <value>
    auto instr = std::make_shared<HLIPushConstantArithmetic>();
    instr->m_type = std::make_shared<OliveType>(TypeDouble);
    instr->double_type = node->value;
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeCallExpression *node) {
    //
    // push ? <function>
    node->callee->accept(this);
    //
    // push ? <arguments>
    for (auto arg : node->arguments) {
        arg->accept(this); }

    // call <#arguments>
    auto instr = std::make_shared<HLICall>();
    instr->numberOfArguments = node->arguments.size();
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeStatementReturn *node) {
    node->expression->accept(this);

    // return <type>
    auto instr = std::make_shared<HLIReturn>();
    instr->m_type = builder()->currentSemanticBlock()->parentFunctionLocal()->returnType;
    builder()->currentBlock()->addInstruction(instr);
}

void HLEmitterVisitor::visit(AST::NodeIdentifier *node) {
    auto resolved = builder()->currentSemanticBlock()->resolveVariableNamed(node->name);
    if (resolved->isLocal()) {
        auto instr = std::make_shared<HLIPushLocalVariableRef>();
        instr->m_type = resolved->type;
        instr->slotidx = std::static_pointer_cast<HLVariableInfoLocal>(resolved)->slotID;
        builder()->currentBlock()->addInstruction(instr);
    }
}

}
}
