//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOScript.hxx"
#include "LOCommon.hxx"
#include "LOCell.hxx"

#include <memory>
#include <assert.h>

namespace Olivia {

std::shared_ptr<LOClass> LOScript::addClass(std::shared_ptr<AST::Node> ast_node) {
    assert(ast_node->kind() == AST::NDeclarationClass);
    std::shared_ptr<AST::NodeDeclarationClass> node = std::static_pointer_cast<AST::NodeDeclarationClass>(ast_node);

    std::string name = node->name->name;
    assert(!hasClassNamed(name));

    std::shared_ptr<LOClass> ret = std::make_shared<LOClass>();
    ret->name = name;

    for (auto i : node->members) {
        switch (i->kind()) {
            case AST::NDeclarationClassElementState: {
                assert(!ret->hasMemberNamed(i->name->name));
                std::shared_ptr<LOVariableSlot> slot = std::make_shared<LOVariableSlot>();
                slot->name = i->name->name;
                //                slot->offset = ret->getLastOffset();
                OliveType type;
                type.setBaseType(i->type->base_type);
                if (type.baseType() == TypeObject) {
                    type.setConcreteName(i->type->concrete_type->name);
                }
                slot->type = type;

                LOG("Adding variable %s typed <%s> to class '%s' ...\n", slot->name.c_str(),
                        convertValueTypeToString(slot->type.baseType()), ret->name.c_str());
                ret->addVariable(slot);
            }
                break;
            default:
                assert(false);
                return nullptr;
        }
    }

    custom_types[name] = ret;
    return ret;
}

bool LOScript::hasClassNamed(const std::string& name) {
    auto i = custom_types.find(name);
    if (i != custom_types.end()) {
        return true;
    }
    return false;
}

void LOScript::finishupDeclarations() {
    for (auto class_pair : custom_types) {
        for (auto var : class_pair.second->variables) {
            if (!var->type.finished()) {
                var->type.findConcrete(this);
            }
        }
    }
}

bool OliveType::findConcrete(LOScript *script) {
    assert(script);
    if (baseType() == TypeObject) {
        if (script->hasClassNamed(m_concrete_tmp)) {
            m_concrete = script->getClassNamed(m_concrete_tmp);
            return true;
        } else { return false; }
    }
}

std::shared_ptr<LOClass> LOScript::getClassNamed(const std::string& name) {
    auto i = custom_types.find(name);
    if (i != custom_types.end()) {
        return i->second;
    } else { return nullptr; }
}

void LOScript::addGlobalFunction(std::shared_ptr<AST::NodeDeclarationFunction> ast_node) {
    std::string name = ast_node->name->name;
    assert(!hasGlobalFunctionNamed(name));
    LOG("Adding global function %s returning <%s> ...\n", ast_node->name->name.c_str(),
            convertValueTypeToString(ast_node->signature->return_type->base_type));
    global_functions[name] = ast_node;
}

void LOScript::addExternFunction(std::shared_ptr<AST::NodeDeclarationExternFunction> ast_node) {
    std::string name = ast_node->name->name;
    assert(!hasGlobalFunctionNamed(name));
    LOG("Adding extern function %s returning <%s> ...\n", ast_node->name->name.c_str(),
            convertValueTypeToString(ast_node->signature->return_type->base_type));
    global_functions[name] = ast_node;
}

bool LOScript::hasGlobalFunctionNamed(const std::string& name) {
    auto i = global_functions.find(name);
    if (i == global_functions.end()) {
        return false;
    } else { return true; }
}

std::shared_ptr<AST::NodeDeclaration> LOScript::getGlobalFunctionNamed(const std::string& name) {
    auto i = global_functions.find(name);
    if (i == global_functions.end()) {
        return nullptr;
    } else { return i->second; }
}

ResolveResult LOScript::resolveIdentifier(AST::NodeIdentifier *id) {
    ResolveResult ret;
    ret.type = RNone, ret.node = nullptr;

    if (hasGlobalFunctionNamed(id->name)) {
        ret.type = RGlobalFunction;
        ret.node = getGlobalFunctionNamed(id->name);
        return ret;
    }

    return ret;
}

}
