#pragma once

#include <string>

enum class TokenType
{
    Integer,
    Double,
    Ident,
    Keyword,
    Add,
    Inc,
    AddAssign,
    Assign,
    Equal,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    OpenSquareBracket,
	CloseSquareBracket,
	Comma,
    Semicolon,
    Sub,
    Dec,
    SubAssign,
    Mul,
    MulAssign,
    Div,
    DivAssign,
    Rem,
    RemAssign,
    Not,
    NotEqual,
    LessThan,
	LargerThan,
	LessEqualThan,
	LargerEqualThan,
    BitAnd,
    And,
    AndAssign,
    BitOr,
    Or,
    OrAssign,
    LeftShift,
    LeftShiftAssign,
    RightShift,
    RightShiftAssign,
    End
};

struct Token
{
    TokenType type;
    std::string val;
    int row, col;

    Token();
    Token(TokenType type, const std::string& val, int row, int col);
    std::string toString() const;
    static std::string GetTypeSymbol(TokenType type);
};
