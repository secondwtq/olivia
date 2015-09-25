//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOTYPE_HXX
#define VMFOROLIVE_LOTYPE_HXX

#include <memory>
#include "LOAST.hxx"
#include "LOScript.hxx"

namespace Olivia {

struct OliveType;
namespace AST {
class NodeTypeSpec;
}
class LOScript;

std::shared_ptr<OliveType> convertTypeSpecNodeToOliveType(
        const AST::NodeTypeSpec& type, LOScript &script);

std::shared_ptr<OliveType> convertSignatureToOliveType(
        const AST::NodeDeclarationSignature& type, LOScript &script);

std::shared_ptr<OliveType> convertNodeToOliveType(
        const AST::Node& type, LOScript& script);

}

#endif // VMFOROLIVE_LOTYPE_HXX
