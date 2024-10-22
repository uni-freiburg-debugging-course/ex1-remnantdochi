#pragma once
#include <string>

enum TokenType
{
    LPAR,
    RPAR,
    KEY,
    OPR,
    NUM,
    UNK
};

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v) : type(t), value(v) {}
};