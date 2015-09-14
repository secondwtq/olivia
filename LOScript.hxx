//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOSCRIPT_HXX
#define VMFOROLIVE_LOSCRIPT_HXX

#include <unordered_map>
#include <string>
#include <memory>

#include "LOValue.hxx"
#include "LOAST.hxx"

namespace Olivia {

class LOClass;

class LOScript {
public:

    std::shared_ptr<LOClass> getClassNamed(const std::string& name);
    bool hasClassNamed(const std::string& name);
    std::shared_ptr<LOClass> addClass(std::shared_ptr<AST::Node> ast_node);

    void finishupDeclarations();

    std::unordered_map<std::string, LOValue> global_vars;
    std::unordered_map<std::string, std::shared_ptr<LOClass>> custom_types;
};

}

#endif // VMFOROLIVE_LOSCRIPT_HXX
