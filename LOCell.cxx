//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOCell.hxx"

#include <assert.h>
#include <string>

namespace Olivia {

LOObject *LOObject::Alloc(LOClass *klass) {
    assert(klass);
    LOObject *ret = new LOObject();
    ret->klass = klass;
}

size_t LOClass::getLastOffset() const {
    return 8 * variables.size();
}

bool LOClass::hasMemberNamed(const std::string& name) const {
    auto i = variable_map.find(name);
    if (i != variable_map.end()) {
        return true;
    } else { return false; }
}

void LOClass::addVariable(std::shared_ptr<LOVariableSlot> slot) {
    assert(!hasMemberNamed(slot->name));
    slot->offset = getLastOffset();
    variable_map[slot->name] = slot;
    variables.push_back(slot);
}

}
