//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOASTVisitor.hxx"
#include "LOAST.hxx"

namespace Olivia {
namespace AST {

void Node::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodePrefixUnaryExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeParenthesisExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeIdentifier::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeMemberExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeBinaryExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeCallExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeStatementExpression::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeStatementReturn::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeBlock::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeStatementIf::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeStatementWhile::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclaration::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeTypeSpec::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeStatementVar::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationVar::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void MiscVarDeclarationList::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationClassElement::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationClassElementState::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationClassElementMethod::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationParameter::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationSignature::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationFunction::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationExternFunction::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeDeclarationClass::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeConstantBoolean::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeConstantDouble::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeConstantInteger::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

void NodeConstantString::accept(LOASTVisitor *visitor) {
    visitor->visit(this); }

}

}