//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/11.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef VMFOROLIVE_LOLEXER_HXX
#define VMFOROLIVE_LOLEXER_HXX

#include <stdint.h>
#include <stddef.h>

#include <string>
#include <unordered_map>

namespace Olivia {

enum LexerKeyTokenType {
    TEnd = 0,

    TIdentifier = -1,
    TStringLiteral = -2,
    TInteger = -3,
    TFloat = -4,

    TVar = -6,
    TReturn = -7,

    TIf = -8,
    TElse = -9,

    TFor = -10,
    TBreak = -11,
    TContinue = -12,

    TWhile = -13,
    TDo = -14,

    TSwitch = -15,
    TCase = -16,
    TDefault = -17,

    TClass = -18,
    TExtends = -19,
    TConstructor = -20,
    TThis = -21,

    TFunction = -22,

    TTripleDot = -23,
    TOperator = -24,

    TInplaceDiv = -25,

    TBoolean = -26,
    TDouble = -27,
    TInt32 = -28,
    TUInt32 = -29,
    TInt8 = -30,
    TString = -31,

    TNew = -32,
    TVoid = -33,
    TExtern = -34,

    TTrue = -35,
    TFalse = -36,

    TInplaceAdd = -37,
    TInplaceSub = -38,
    TInplaceMul = -39,

    TIncrement = -40,
    TDecrement = -41,
    TEqual = -42,
    TNotEqual = -43,

    TRawString = -44,

    TLogicalAnd = -45,
    TLogicalOr = -46,

    TGreaterEqual = -47,
    TLessEqual = -48
};

typedef int16_t LexerTokenType;

class LOLexer {
public:

    LOLexer();

    LexerTokenType lex();

    void lex_comment_block();
    void lex_comment_line();

    LexerTokenType lex_number();
    LexerTokenType lex_ansequence();

    void advance();

    void emit_error(const char *message);

    void init_keyword_map();

    size_t curline = 0;
    size_t curcol = 1;
    unsigned char prevchr = 0;
    unsigned char curchr = 0;
    LexerTokenType prevtok;
    LexerTokenType curtok;

    const unsigned char *dataptr = nullptr;

    std::unordered_map<std::string, LexerTokenType> keyword_map;
    bool reached_eof = false;
    double cache_float = 0.0;
    int64_t cache_integer = 0;
    std::string cache_id;
};

}

#endif // VMFOROLIVE_LOLEXER_HXX
