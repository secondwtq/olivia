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
    return ret;
}

}
}
