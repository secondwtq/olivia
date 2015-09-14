//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOPARSER_HXX
#define VMFOROLIVE_LOPARSER_HXX

#include "LOLexer.hxx"
#include "LOAST.hxx"

#include <memory>
#include <unordered_map>

namespace Olivia {

class LOScript;

class LOParser {
public:

    LOParser();

    std::shared_ptr<AST::NodeIdentifier> parse_identifier(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeStatement> parse_statement(std::shared_ptr<AST::Node> parent);
    void parse_class_members();
    std::shared_ptr<AST::NodeDeclarationClass> parse_class_declaration(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeDeclarationClassElement> parse_class_element(std::shared_ptr<AST::Node> parent);
    std::vector<std::shared_ptr<AST::NodeDeclarationClassElement>> parse_class_elements(std::shared_ptr<AST::Node> parent);

    std::shared_ptr<AST::NodeStatementVar> parse_var_statement(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::MiscVarDeclarationList> parse_var_decllist(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeDeclarationVar> parse_var_declaration(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeTypeSpec> parse_type(std::shared_ptr<AST::Node> parent, bool colon = true);

    std::shared_ptr<AST::NodeDeclarationFunction> parse_function_declaration(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeDeclarationExternFunction> parse_function_extern_declaration(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeDeclarationSignature> parse_function_signature(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeBlock> parse_block(std::shared_ptr<AST::Node> parent);

    std::shared_ptr<AST::NodeExpression> parse_expression(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeExpression> parse_expression_binary(std::shared_ptr<AST::Node> parent, int precedence);
    std::shared_ptr<AST::NodeExpression> parse_expression_binary_rhs(
            std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeExpression> lhs, int precedence);
    std::shared_ptr<AST::NodeExpression> parse_expression_postfix(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodePrimaryExpression> parse_expression_primary(
            std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeExpression> parse_expression_unary(std::shared_ptr<AST::Node> parent);

    std::shared_ptr<AST::NodeExpression> parse_expression_maybe_call(
            std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeLHSExpression> expression);
    std::shared_ptr<AST::NodeExpression> parse_expression_maybe_member(
            std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeLHSExpression> expression);
    std::vector<std::shared_ptr<AST::NodeExpression>> parse_call_arguments(std::shared_ptr<AST::Node> parent);

    std::shared_ptr<AST::NodeParenthesisExpression> parse_parenthesis(std::shared_ptr<AST::Node> parent);
    std::shared_ptr<AST::NodeStatementExpression> parse_expression_statement(std::shared_ptr<AST::Node> parent);

    int token_precedence(LexerTokenType t) const;

    std::shared_ptr<LOScript> script;

    void parse_semicolon();

    void error(const char *message);

    LexerTokenType lex() {
        return (token = lexer->lex()); }
    bool expect(LexerTokenType expected_token, bool advance = true);

    LexerTokenType token;
    LOLexer *lexer;

private:

    void init_token_predmap();
    std::unordered_map<LexerTokenType, int> m_token_precedence;
};

}

#endif // VMFOROLIVE_LOPARSER_HXX
