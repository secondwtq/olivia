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

#include <memory>

namespace Olivia {
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
}
}

namespace Olivia {
namespace HL {

class HLBlockBuilder {
public:

    HLBlockBuilder(std::shared_ptr<HLScript> script) : m_script(script) { }

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



private:
    std::shared_ptr<HLBlock> m_current_block;
    std::weak_ptr<HLScript> m_script;
};

template <typename T>
class HLSimulatorStack {
public:

};

}
}

#endif // VMFOROLIVE_LOHLBUILDER_HXX
