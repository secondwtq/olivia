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
            *node->signature, *builder()->parentScript()->lscript), true);
}

void HLEmitterVisitor::visit(AST::NodeDeclarationFunction *node) {
    // alloc a Constant Global for functions in default
    builder()->currentSemanticBlock()->allocNameStatic(
            node->name->name, convertSignatureToOliveType(
            *node->signature, *builder()->parentScript()->lscript), true);

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
    builder()->addPop();
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
    //    auto semantic_block = builder()->currentSemanticBlock()->appendSemanticBlock();
    //    auto block = semantic_block->openBlock();
    //    builder()->setCurrentBlock(block);
    for (auto statement : node->statements) {
        statement->accept(this); }
}

void HLEmitterVisitor::visit(AST::NodeConstantBoolean *node) {
    // push constant boolean <value>
    builder()->addPushConstantArithmeticBoolean(node->value);
}

void HLEmitterVisitor::visit(AST::NodeConstantInteger *node) {
    // push constant int32 <value>
    builder()->addPushConstantArithmeticInteger(node->value);
}

void HLEmitterVisitor::visit(AST::NodeConstantDouble *node) {
    // push constant double <value>
    builder()->addPushConstantArithmeticDouble(node->value);
}

void HLEmitterVisitor::visit(AST::NodeCallExpression *node) {
    //
    // push ? <function>
    node->callee->accept(this);
    auto func_type = builder()->sstack->top()->type;
    //
    // push ? <arguments>
    for (size_t i = 0; i < node->arguments.size(); i++) {
        node->arguments[i]->accept(this);

        castTopValueIfNeeded(func_type->callArguments[i]);
    }

    // call <#arguments>
    builder()->addCall(node->arguments.size());
}

void HLEmitterVisitor::visit(AST::NodeStatementReturn *node) {
    node->expression->accept(this);

    // return <type>
    auto return_type = builder()->currentSemanticBlock()->parentFunctionLocal()->returnType;
    if (return_type->baseType() != TypeVoid) {
        castTopValueIfNeeded(return_type); }
    builder()->addReturn(return_type);
}

void HLEmitterVisitor::visit(AST::NodeIdentifier *node) {
    auto resolved = builder()->currentSemanticBlock()->resolveVariableNamed(node->name);
    if (resolved->isLocal()) {
        builder()->addPushLocalVariableRef(resolved->type,
                std::static_pointer_cast<HLVariableInfoLocal>(resolved)->slotID);
    } else if (resolved->isConstant()) {
        builder()->addPushConstantGlobal(resolved->type, node->name);
    } else {
        auto instr = std::make_shared<HLIPushGlobalVariableRef>();
        instr->m_type = resolved->type;
        builder()->currentBlock()->addInstruction(instr);
    }
}

void HLEmitterVisitor::visit(AST::NodeStatementIf *node) {
    node->cond_->accept(this);
    castTopValueIfNeeded(std::make_shared<OliveType>(TypeBoolean));

    std::shared_ptr<HLBlock> block_then = builder()->currentSemanticBlock()->appendSemanticBlock()->openBlock();
    std::shared_ptr<HLBlock> block_else = node->hasElse() ? builder()->currentSemanticBlock()->
            appendSemanticBlock()->openBlock() : nullptr;
    std::shared_ptr<HLBlock> block_cont = builder()->currentSemanticBlock()->openBlock();
    builder()->addBranchConditionBinary(block_then, node->hasElse() ? block_else : block_cont);

    builder()->setCurrentBlock(block_then);
    node->then_->accept(this);
    builder()->addBranchJump(block_cont);

    if (node->hasElse()) {
        builder()->setCurrentBlock(block_else);
        node->else_->accept(this);
        builder()->addBranchJump(block_cont);
    }

    builder()->setCurrentBlock(block_cont);
}

void HLEmitterVisitor::visit(AST::NodeStatementWhile *node) {
    std::shared_ptr<HLBlock> block_cond = builder()->currentSemanticBlock()->openBlock();
    std::shared_ptr<HLBlock> block_body = builder()->currentSemanticBlock()->appendSemanticBlock()->openBlock();
    std::shared_ptr<HLBlock> block_cont = builder()->currentSemanticBlock()->openBlock();
    builder()->addBranchJump(block_cond);
    builder()->setCurrentBlock(block_cond);
    node->cond->accept(this);
    castTopValueIfNeeded(std::make_shared<OliveType>(TypeBoolean));
    builder()->addBranchConditionBinary(block_body, block_cont);
    builder()->setCurrentBlock(block_body);
    node->body->accept(this);
    builder()->addBranchJump(block_cond);
    builder()->setCurrentBlock(block_cont);
}

void HLEmitterVisitor::castTopValueIfNeeded(std::shared_ptr<OliveType> type) {
    auto current_type = builder()->sstack->top()->type;
    if (!current_type->equalsEffectively(type)) {
        builder()->addCast(current_type, type); }
    assert(builder()->sstack->top()->type->equalsEffectively(type));
}

void HLEmitterVisitor::visit(AST::NodeParenthesisExpression *node) {
    node->expression->accept(this); }

bool isTypeArithmetic(const std::shared_ptr<OliveType> type) {
    auto base = type->baseType();
    switch (base) {
        case TypeInt8:
            return true;
        case TypeInt32:
            return true;
        case TypeUInt32:
            return true;
        case TypeBoolean:
            return true;
        case TypeDouble:
            return true;
        default:
            return false;
    }
}

// binary operators: + - * / % ^
//  = == < > <= >=
//  += -= *= /= %=
//  & |
//  && ||

std::shared_ptr<HLIBase> insertCast(std::shared_ptr<HLBlockBuilder> builder,
        std::shared_ptr<OliveType> from, std::shared_ptr<OliveType> to,
        std::shared_ptr<HLIBase> instr_pos, size_t stack_pos) {
    auto ret = std::make_shared<HLICast>();
    ret->from = from, ret->to = to;
    builder->currentBlock()->insertInstruction(instr_pos, ret);
    builder->sstack->queue[stack_pos] = HLSimulatorStackValue::createTemporary(to);
    return ret;
}

void unifyBinaryExpressionType(std::shared_ptr<HLBlockBuilder> builder,
        std::shared_ptr<HLIBase> lhs) {
    auto typeRHS = builder->sstack->indexed(0)->type,
            typeLHS = builder->sstack->indexed(1)->type;

    auto dominateValueType = [&] (LOValueType t) {
        if (typeLHS->baseType() == t || typeRHS->baseType() == t) {
            if (typeLHS->baseType() == t) {
                builder->addCast(typeRHS, typeLHS);
            } else { insertCast(builder, typeLHS, typeRHS, lhs, 1); }
        }
        typeRHS = builder->sstack->indexed(0)->type,
                typeLHS = builder->sstack->indexed(1)->type;
    };

    while (isTypeArithmetic(typeLHS) && isTypeArithmetic(typeRHS)) {
        if (typeLHS->equalsEffectively(typeRHS)) { break; }
        // TODO: refactor to be more descriptive
        dominateValueType(TypeDouble);
        dominateValueType(TypeUInt32);
        dominateValueType(TypeInt32);
        dominateValueType(TypeInt8);
        dominateValueType(TypeBoolean);
        break;
    }
}

void HLEmitterVisitor::visit(AST::NodeBinaryExpression *node) {
    node->lhs->accept(this);
    auto lhspos = builder()->currentBlock()->topInstruction();
    node->rhs->accept(this);

    unifyBinaryExpressionType(builder(), lhspos);

    auto toptype = builder()->sstack->top()->type;
    switch (node->op) {
        case '+':
            builder()->addArithmeticBinaryAdd(toptype);
            break;
    }
}

}
}
