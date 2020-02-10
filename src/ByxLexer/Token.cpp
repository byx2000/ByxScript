#include "Token.h"

using namespace std;

static string typeDesc[] =
{
    "Integer",
    "Double",
    "Ident",
    "Keyword",
    "Add",
    "Inc",
    "AddAssign",
    "Assign",
    "Equal",
    "OpenBracket",
    "CloseBracket",
    "OpenBrace",
    "CloseBrace",
    "OpenSquareBracket",
    "CloseSquareBracket",
    "Comma",
    "Semicolon",
    "Sub",
    "Dec",
    "SubAssign",
    "Mul",
    "MulAssign",
    "Div",
    "DivAssign",
    "Rem",
    "RemAssign",
    "Not",
    "NotEqual",
    "LessThan",
	"LargerThan",
	"LessEqualThan",
	"LargerEqualThan",
    "BitAnd",
    "And",
    "AndAssign",
    "BitOr",
    "Or",
    "OrAssign",
    "LeftShift",
    "LeftShiftAssign",
    "RightShift",
    "RightShiftAssign",
    "End"
};

static string typeSymbol[] =
{
    "integer",
    "double",
    "identifier",
    "Keyword",
    "+",
    "++",
    "+=",
    "=",
    "==",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
    ",",
    ";",
    "-",
    "--",
    "-=",
    "*",
    "*=",
    "/",
    "/=",
    "%",
    "%=",
    "!",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "&",
    "&&",
    "&=",
    "|",
    "||",
    "|=",
    "<<",
    "<<=",
    ">>",
    ">>=",
    "end"
};

Token::Token() : type(TokenType::End), val(""), row(0), col(0)
{

}

Token::Token(TokenType type, const std::string& val, int row, int col) : type(type), val(val), row(row), col(col)
{

}

std::string Token::toString() const
{
    string s = "<";
    s += typeDesc[(int)type];
    s += ",";
    s += val;
    s += ">";
    return s;
}

std::string Token::GetTypeSymbol(TokenType type)
{
    return typeSymbol[(int)type];
}
