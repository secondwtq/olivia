//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOASTVISITOR_HXX
#define VMFOROLIVE_LOASTVISITOR_HXX

namespace Olivia {

namespace AST {
class NodeStatementVar;
}

class LOASTVisitor {
public:

    void visit(AST::NodeStatementVar *node);
};

}

#endif // VMFOROLIVE_LOASTVISITOR_HXX
