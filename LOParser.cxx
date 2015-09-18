//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/12.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOParser.hxx"
#include "LOLexer.hxx"
#include "LOAST.hxx"

#include <memory>

namespace Olivia {

LOParser::LOParser() {
    init_token_predmap(); }

std::shared_ptr<AST::NodeStatement> LOParser::parse_statement(std::shared_ptr<AST::Node> parent) {
    switch (token) {
        case ';':
            lex(); break;
        case TIf:
            break;
        case TWhile:
            break;
        case TDo:
            break;
        case TFor:
            break;
        case TSwitch:
            break;
        case TVar:
            return parse_var_statement(parent);
        case TClass:
            return parse_class_declaration(parent);
        case TFunction:
            return parse_function_declaration(parent);
        case TExtern:
            return parse_function_extern_declaration(parent);
        case TReturn:
            return parse_return_statement(parent);
        case '{':
            break;
        default:
            return parse_expression_statement(parent);
    }
    return nullptr;
}

std::shared_ptr<AST::NodeDeclarationClass> LOParser::parse_class_declaration(std::shared_ptr<AST::Node> parent) {
    auto ret = std::make_shared<AST::NodeDeclarationClass>(parent);
    expect(TClass);
    ret->name = parse_identifier(ret);
    expect('{');
    ret->members = std::move(parse_class_elements(ret));
    expect('}');
    return ret;
}

bool LOParser::expect(LexerTokenType expected_token, bool advance) {
    if (expected_token == token) {
        if (advance) {
            lex(); }
        return true;
    }
    std::string error_message = "expected '";
    error_message += expected_token;
    error_message += "'.";
    error(error_message.c_str());
    return false;
}

std::shared_ptr<AST::NodeIdentifier> LOParser::parse_identifier(std::shared_ptr<AST::Node> parent) {
    if (token != TIdentifier) {
        error("expected identifier."); }
    std::string t = lexer->cache_id;
    auto ret = std::make_shared<AST::NodeIdentifier>(parent, t);
    lex();
    return ret;
}

std::vector<std::shared_ptr<AST::NodeDeclarationClassElement>> LOParser::parse_class_elements(std::shared_ptr<AST::Node> parent) {
    std::vector<std::shared_ptr<AST::NodeDeclarationClassElement>> ret;
    while (token != '}') {
        auto t = parse_class_element(parent);
        if (t) {
            ret.push_back(t);
        }
    }
    return std::move(ret);
}

std::shared_ptr<AST::NodeDeclarationClassElement> LOParser::parse_class_element(std::shared_ptr<AST::Node> parent) {
    auto id = parse_identifier(nullptr);
    if (token == '(') {
        auto ret = std::make_shared<AST::NodeDeclarationClassElementMethod>(parent);
        id->set_parent(ret);
        ret->name = id;
    } else {
        auto ret = std::make_shared<AST::NodeDeclarationClassElementState>(parent);
        id->set_parent(ret);
        ret->name = id;
        ret->type = parse_type(ret);
        expect(';');
        return ret;
    }
    return nullptr;
}

std::shared_ptr<AST::NodeStatementVar> LOParser::parse_var_statement(std::shared_ptr<AST::Node> parent) {
    auto ret = std::make_shared<AST::NodeStatementVar>(parent);
    ret->list = parse_var_decllist(ret);
    expect(';');
    return ret;
}

std::shared_ptr<AST::MiscVarDeclarationList> LOParser::parse_var_decllist(std::shared_ptr<AST::Node> parent) {
    auto ret = std::make_shared<AST::MiscVarDeclarationList>(parent);
    lex();
    while (1) {
        auto t = parse_var_declaration(ret);
        ret->vars.push_back(t);
        if (token == ';') {
            break;
        } else {
            expect(',');
        }
    }
    return ret;
}

std::shared_ptr<AST::NodeDeclarationVar> LOParser::parse_var_declaration(std::shared_ptr<AST::Node> parent) {
    auto ret = std::make_shared<AST::NodeDeclarationVar>(parent);
    ret->name = parse_identifier(ret);
    ret->type = parse_type(ret);
    return ret;
}

std::shared_ptr<AST::NodeTypeSpec> LOParser::parse_type(std::shared_ptr<AST::Node> parent, bool colon) {
    auto ret = std::make_shared<AST::NodeTypeSpec>(parent);
    if (colon) {
        expect(':'); }
    switch (token) {
        case TVoid:
            ret->base_type = TypeVoid;
            lex();
            break;
        case TBoolean:
            ret->base_type = TypeBoolean;
            lex();
            break;
        case TDouble:
            ret->base_type = TypeDouble;
            lex();
            break;
        case TInt32:
            ret->base_type = TypeInt32;
            lex();
            break;
        case TInt8:
            ret->base_type = TypeInt8;
            lex();
            break;
        case TUInt32:
            ret->base_type = TypeUInt32;
            lex();
            break;
        case TString:
            ret->base_type = TypeString;
            lex();
            break;
        case TIdentifier:
            ret->base_type = TypeObject;
            ret->concrete_type = parse_identifier(ret);
            break;
        default:
            error("invalid type spec");
            break;
    }
    return ret;
}

void LOParser::error(const char *message) {
    printf("%s\n", message);
}

std::shared_ptr<AST::NodeDeclarationSignature> LOParser::parse_function_signature(
        std::shared_ptr<AST::Node> parent, const std::string& name) {
    expect('(');
    auto ret = std::make_shared<AST::NodeDeclarationSignature>(parent);
    ret->trusted_name = name;
    while (token != ')') {
        auto p = std::make_shared<AST::NodeDeclarationParameter>(ret);
        p->name = parse_identifier(p);
        p->type = parse_type(p);
        ret->parameters.push_back(p);

        if (token == ')') {
            break;
        } else { expect(','); }
    }
    expect(')');
    ret->return_type = parse_type(ret);
    return ret;
}

std::shared_ptr<AST::NodeDeclarationFunction> LOParser::parse_function_declaration(std::shared_ptr<AST::Node> parent) {
    lex();
    auto ret = std::make_shared<AST::NodeDeclarationFunction>(parent);
    ret->name = parse_identifier(ret);
    ret->signature = parse_function_signature(ret, ret->name->name);
    ret->body = parse_block(ret);
    return ret;
}

std::shared_ptr<AST::NodeDeclarationExternFunction> LOParser::parse_function_extern_declaration(
        std::shared_ptr<AST::Node> parent) {
    lex();
    expect(TFunction);
    auto ret = std::make_shared<AST::NodeDeclarationExternFunction>(parent);
    ret->name = parse_identifier(ret);
    ret->signature = parse_function_signature(ret, ret->name->name);
    expect(';');
    return ret;
}

std::shared_ptr<AST::NodeBlock> LOParser::parse_block(std::shared_ptr<AST::Node> parent) {
    expect('{');
    auto ret = std::make_shared<AST::NodeBlock>(parent);
    while (token != '}') {
        auto t = parse_statement(ret);
        ret->statements.push_back(t);
    }
    expect('}');
    return ret;
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_unary(std::shared_ptr<AST::Node> parent) {
    switch (token) {
        case '+':
        case '-': {
            auto ret = std::make_shared<AST::NodePrefixUnaryExpression>(parent);
            ret->op = token;
            lex();
        }
        default:
            return parse_expression_postfix(parent);
    }
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_postfix(std::shared_ptr<AST::Node> parent) {
    auto ret = parse_expression_primary(parent);
    return parse_expression_maybe_call(parent, ret);
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_maybe_call(
        std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeLHSExpression> expression) {
    auto e = expression;
    while (true) {
        if (token == '(') {
            auto ret = std::make_shared<AST::NodeCallExpression>(parent);
            ret->callee = e;
            ret->arguments = std::move(parse_call_arguments(ret));

            e->set_parent(ret);
            e = ret;
            continue;
        }
        return e;
    }
}

std::vector<std::shared_ptr<AST::NodeExpression>> LOParser::parse_call_arguments(
        std::shared_ptr<AST::Node> parent) {
    expect('(');
    std::vector<std::shared_ptr<AST::NodeExpression>> ret;
    while (token != ')') {
        auto t = parse_expression(parent);
        if (t) {
            ret.push_back(t); }
        if (token == ')') {
            break;
        } else { expect(','); }
    }
    expect(')');
    return std::move(ret);
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_maybe_member(
        std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeLHSExpression> expression) {
    auto e = expression;
    while (true) {
        if (token == '.') {
            auto ret = std::make_shared<AST::NodeMemberExpression>(parent);
            ret->expression = expression;
            expect('.');
            ret->name = parse_identifier(ret);

            expression->set_parent(ret);
            e = ret;
            continue;
        }
        return e;
    }
}

std::shared_ptr<AST::NodePrimaryExpression> LOParser::parse_expression_primary(
        std::shared_ptr<AST::Node> parent) {
    switch (token) {
        case TIdentifier:
            return parse_identifier(parent);
        case '(':
            return parse_parenthesis(parent);
        case TTrue: {
            auto ret = std::make_shared<AST::NodeConstantBoolean>(parent);
            ret->value = true;
            lex();
            return ret;
        }
        case TFalse: {
            auto ret = std::make_shared<AST::NodeConstantBoolean>(parent);
            ret->value = false;
            lex();
            return ret;
        }
        case TInteger: {
            auto ret = std::make_shared<AST::NodeConstantInteger>(parent);
            ret->value = lexer->cache_integer;
            lex();
            return ret;
        }
        case TFloat: {
            auto ret = std::make_shared<AST::NodeConstantDouble>(parent);
            ret->value = lexer->cache_float;
            lex();
            return ret;
        }
    }
    return nullptr;
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression(std::shared_ptr<AST::Node> parent) {
    auto b = parse_expression_binary(parent, 0);
    if (b->isLHS() && (token == '=')) {
        auto ret = std::make_shared<AST::NodeBinaryExpression>(parent);
        expect('=');
        ret->op = '=';
        b->set_parent(ret);
        ret->lhs = b;

        auto rhs = parse_expression(ret);
        ret->rhs = rhs;
        return ret;
    }
    return b;
}

std::shared_ptr<AST::NodeParenthesisExpression> LOParser::parse_parenthesis(
        std::shared_ptr<AST::Node> parent) {
    expect('(');
    auto ret = std::make_shared<AST::NodeParenthesisExpression>(parent);
    auto e = parse_expression(ret);
    ret->expression = e;
    expect(')');
    return ret;
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_binary(std::shared_ptr<AST::Node> parent, int precedence) {
    auto lhs = parse_expression_unary(parent);
    if (!lhs) {
        return nullptr; }
    return parse_expression_binary_rhs(parent, lhs, precedence);
}

std::shared_ptr<AST::NodeExpression> LOParser::parse_expression_binary_rhs(
        std::shared_ptr<AST::Node> parent, std::shared_ptr<AST::NodeExpression> lhs, int precedence) {
    while (1) {
        LexerTokenType binary_op = token;
        int curr_prec = token_precedence(binary_op);
        if (curr_prec <= precedence) {
            return lhs; }

        lex();
//        auto rhs = parse_expression_unary(parent);
//        if (!rhs) {
//            return nullptr; }

//        int next_prec = token_precedence(token);
//        if (curr_prec < next_prec) {
//            rhs = parse_expression_binary_rhs(parent, rhs, curr_prec+1);
//            if (!rhs) {
//                return nullptr; }
//        }

        auto bin_exp = std::make_shared<AST::NodeBinaryExpression>(lhs->parent());
        bin_exp->op = binary_op;
        lhs->set_parent(bin_exp);
        bin_exp->lhs = lhs;

        auto rhs = parse_expression_binary(bin_exp, precedence);
        bin_exp->rhs = rhs;
//        rhs->set_parent(bin_exp);

        lhs = bin_exp;
    }
}

int LOParser::token_precedence(LexerTokenType t) const {
    auto i = m_token_precedence.find(t);
    if (i == m_token_precedence.end()) {
        return -1; }
    int ret = i->second;
    if (ret <= 0) {
        return -1; }
    return ret;
}

void LOParser::init_token_predmap() {
    m_token_precedence['<'] = 7;
    m_token_precedence['>'] = 7;

    m_token_precedence['+'] = 9;
    m_token_precedence['-'] = 9;

    m_token_precedence['*'] = 10;
    m_token_precedence['/'] = 10;
    m_token_precedence['%'] = 10;
}

std::shared_ptr<AST::NodeStatementExpression> LOParser::parse_expression_statement(
        std::shared_ptr<AST::Node> parent) {
    auto ret = std::make_shared<AST::NodeStatementExpression>(parent);
    auto e = parse_expression(ret);
    ret->expression = e;
    expect(';');
    return ret;
}

std::shared_ptr<AST::NodeStatementReturn> LOParser::parse_return_statement(std::shared_ptr<AST::Node> parent) {
    expect(TReturn);
    auto ret = std::make_shared<AST::NodeStatementReturn>(parent);
    ret->expression = parse_expression(ret);
    expect(';');
    return ret;
}

}
