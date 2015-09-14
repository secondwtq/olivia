//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOCELL_HXX
#define VMFOROLIVE_LOCELL_HXX

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "LOValue.hxx"

namespace Olivia {

class LOVariableSlot {
public:
    std::string name;
    std::size_t offset;
    OliveType type;
};

class LOClass {
public:

    std::string name;
    std::shared_ptr<LOClass> base;

    void addVariable(std::shared_ptr<LOVariableSlot> slot);

    bool hasMemberNamed(const std::string& name) const;
    size_t getLastOffset() const;

    std::vector<std::shared_ptr<LOVariableSlot>> variables;
    std::unordered_map<std::string, std::shared_ptr<LOVariableSlot>> variable_map;
};

class LOCell {
public:
};

class LOObject : public LOCell {
public:
    static LOObject * Alloc(LOClass *klass);

    LOClass *klass;
};

}

#endif // VMFOROLIVE_LOCELL_HXX
