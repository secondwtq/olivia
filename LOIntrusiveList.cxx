//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include <memory>
#include "LOIntrusiveList.hxx"

namespace Olivia {
namespace Base {

struct TestInt : public IntrusiveListNode<TestInt, std::shared_ptr<TestInt>> {
    int a;
};

void t() {
    IntrusiveList<TestInt> t;
    t.erase(nullptr);
}

}
}
