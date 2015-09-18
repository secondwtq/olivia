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

enum ResolveResultType {
    RNone,
    RGlobalFunction
};

struct ResolveResult {
    ResolveResultType type;
    std::shared_ptr<AST::Node> node;
};

class LOScript {
public:

    std::shared_ptr<LOClass> getClassNamed(const std::string& name);
    bool hasClassNamed(const std::string& name);
    std::shared_ptr<LOClass> addClass(std::shared_ptr<AST::Node> ast_node);

    void addGlobalFunction(std::shared_ptr<AST::NodeDeclarationFunction> ast_node);
    void addExternFunction(std::shared_ptr<AST::NodeDeclarationExternFunction> ast_node);
    bool hasGlobalFunctionNamed(const std::string& name);
    std::shared_ptr<AST::NodeDeclaration> getGlobalFunctionNamed(const std::string& name);

    void finishupDeclarations();

    ResolveResult resolveIdentifier(AST::NodeIdentifier *id);

    std::unordered_map<std::string, std::shared_ptr<AST::NodeDeclaration>> global_functions;
    std::unordered_map<std::string, LOValue> global_vars;
    std::unordered_map<std::string, std::shared_ptr<LOClass>> custom_types;
};

}

#endif // VMFOROLIVE_LOSCRIPT_HXX
