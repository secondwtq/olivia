//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "LOLexer.hxx"
#include <ctype.h>

#include <string>

namespace Olivia {

#define LEXRET(t) { prevtok = curtok; curtok = t; return t; }

bool isLegalIdentifierStarter(LexerTokenType t) {
    return isalpha(t) || t == '_' || t == '$' || t == '#' || t == '?' || t == '@'; }

bool isLegalIdentifierCharacter(LexerTokenType t) {
    return isLegalIdentifierStarter(t) || isdigit(t); }

LexerTokenType LOLexer::lex() {
    while (1) {
        switch (curchr) {
            case TEnd:
                return TEnd;
            case '\t':
            case '\r':
            case ' ':
                advance();
                continue;
            case '\n':
                curline++;
                prevtok = curtok;
                curtok = '\n';
                advance();
                curcol = 1;
                continue;
            case '+':
                advance();
                switch (curchr) {
                    case '=':
                        advance();
                        LEXRET(TInplaceAdd);
                    case '+':
                        advance();
                        LEXRET(TIncrement);
                    default:
                        LEXRET('+');
                }
            case '-':
                advance();
                switch (curchr) {
                    case '=':
                        advance();
                        LEXRET(TInplaceSub);
                    case '-':
                        advance();
                        LEXRET(TDecrement);
                    default:
                    LEXRET('-');
                }
            case '*':
                advance();
                if (curchr == '=') {
                    advance();
                    LEXRET(TInplaceMul);
                }
                LEXRET('*');
            case '/':
                advance();
                switch (curchr) {
                    case '/':
                        advance();
//                        lex_comment_line();
                        continue;
                    case '*':
//                        lex_comment_block();
                        continue;
                    case '=':
                        advance();
                        LEXRET(TInplaceDiv);
                    default:
                        LEXRET('/');
                }
            case '%':
                break;
            case '"':
                break;
            case '\'':
                break;
            case '!':
                break;
            case '=':
                advance();
                switch(curchr) {
                    case '=':
                        LEXRET(TEqual);
                    default:
                        LEXRET('=');
                }
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case ';':
            case ',':
            case '?':
            case '^':
            case '~': {
                LexerTokenType r = curchr;
                advance();
                LEXRET(r);
            }
            default: {
                if (isdigit(curchr)) {
                    LexerTokenType r = lex_number();
                    LEXRET(r);
                } else if (isLegalIdentifierStarter(curchr)) {
                    LexerTokenType r = lex_ansequence();
                    LEXRET(r);
                } else {
                    LexerTokenType r = curchr;
                    if (iscntrl(r)) {
                        std::string t = "unexpected control character (";
                        t += std::to_string((int) r);
                        t += ")";
                        emit_error(t.c_str()); }
                    advance();
                    LEXRET(r);
                }
            }
        }
    }
}

enum LexerNumberType {
    NInteger,
    NFloat,
    NHex,
    NScientific,
    NOctal
};

bool isodigit(int c) {
    return (c >= '0') && (c <= '8'); }

bool isexp(int c) {
    return (c == 'e') || (c == 'E'); }

LexerTokenType LOLexer::lex_number() {
    LexerNumberType type = NInteger;
    LexerTokenType firstchr = curchr;
    std::string buf;
    advance();
    if ((firstchr == '0') && (((toupper(curchr) == 'X') || isodigit(curchr)))) {
        if (isodigit(curchr)) {
            type = NOctal;
            while (isodigit(curchr)) {
                buf.append(1, curchr);
                advance();
            }
            if (isdigit(curchr)) {
                emit_error("invalid octal number"); }
        } else {
            advance();
            type = NHex;
            while (isxdigit(curchr)) {
                buf.append(1, curchr);
                advance();
            }
        }
    } else {
        buf.append(1, firstchr);
        while ((curchr == '.') || isdigit(curchr) || isexp(curchr)) {
            if ((curchr == '.') || isexp(curchr)) {
                type = NFloat;
                if (isexp(curchr)) {
                    if (type != NFloat) {
                        emit_error("invalid numberic format"); }
                    type = NScientific;
                    buf.append(1, curchr);
                    advance();
                    if (curchr == '+' || curchr == '-') {
                        buf.append(1, curchr);
                        advance();
                    }
                    if (!isdigit(curchr)) {
                        emit_error("exponent expected"); }
                }
            }
            buf.append(1, curchr);
            advance();
        }
    }
    switch (type) {
        case NScientific:
        case NFloat:
            cache_float = std::stod(buf);
            return TFloat;
        case NInteger:
            cache_integer = std::stol(buf);
            return TInteger;
        case NHex:
            cache_integer = std::stol(buf, 0, 16);
            return TInteger;
        case NOctal:
            cache_integer = std::stol(buf, 0, 8);
            return TInteger;
    }
    return 0;
}

LexerTokenType LOLexer::lex_ansequence() {
    LexerTokenType r;
    std::string buf;
    do {
        buf.append(1, curchr);
        advance();
    } while (isLegalIdentifierCharacter(curchr));
    auto i = keyword_map.find(buf);
    if (i != keyword_map.end()) {
        r = i->second;
    } else { r = TIdentifier; }
    if (r == TIdentifier) {
        cache_id = buf; }
    return r;
}

void LOLexer::init_keyword_map() {
#define KEYWORD(word, val) keyword_map[#word] = val;

    KEYWORD(function, TFunction);
    KEYWORD(var, TVar);
    KEYWORD(return, TReturn);

    KEYWORD(class, TClass);
    KEYWORD(extends, TExtends);
    KEYWORD(constructor, TConstructor);
    KEYWORD(this, TThis);

    KEYWORD(new, TNew);
    KEYWORD(void, TVoid);

    KEYWORD(boolean, TBoolean);
    KEYWORD(int32, TInt32);
    KEYWORD(double, TDouble);
    KEYWORD(string, TString);
    KEYWORD(rawstring, TRawString);

    KEYWORD(extern, TExtern);

    KEYWORD(true, TTrue);
    KEYWORD(false, TFalse);

    KEYWORD(if, TIf);
    KEYWORD(else, TElse);

    KEYWORD(while, TWhile);

#undef KEYWORD
}

void LOLexer::emit_error(const char *message) {
    puts(message);
}

void LOLexer::advance() {
    LexerTokenType t = *dataptr++;
    if (t != 0) {
        curchr = t;
        curcol++;
        return;
    }
    curchr = TEnd;
    reached_eof = true;
}

LOLexer::LOLexer() {
    init_keyword_map();
}

}
