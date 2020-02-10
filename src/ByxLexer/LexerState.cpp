#include "LexerState.h"
#include "ByxLexer.h"

#include <string>
#include <iostream>
#include <set>
#include <map>

using namespace std;

static set<string> keywords
{
    "int",
    "double",
    "void",
    "if",
    "else",
    "while",
    "for",
    "continue",
    "break",
    "function",
    "return",
    "var",
    "public",
};

static map<char, TokenType> singleToken
{
    pair<char, TokenType>('(', TokenType::OpenBracket),
    pair<char, TokenType>(')', TokenType::CloseBracket),
    pair<char, TokenType>('{', TokenType::OpenBrace),
    pair<char, TokenType>('}', TokenType::CloseBrace),
    pair<char, TokenType>('[', TokenType::OpenSquareBracket),
    pair<char, TokenType>(']', TokenType::CloseSquareBracket),
    pair<char, TokenType>(',', TokenType::Comma),
    pair<char, TokenType>(';', TokenType::Semicolon),
};

static bool IsKeyword(const string& s)
{
    return keywords.count(s) != 0;
}

static bool IsSingleToken(char c)
{
    return singleToken.count(c) != 0;
}

bool BeginState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == 0 || c == ' ' || c == '\t' || c == '\n')
    {
        return true;
    }
    else if (isdigit(c))
    {
        lexer.addToVal(c);
        lexer.setCurrentState(IntegerState);
        return true;
    }
    else if (c == '.')
    {
        lexer.addToVal(c);
        lexer.setCurrentState(DoubleState);
        return true;
    }
    else if (isalpha(c) || c == '_')
    {
        lexer.addToVal(c);
        lexer.setCurrentState(IdentState);
        return true;
    }
    else if (c == '+')
    {
        lexer.setCurrentState(AddState);
        return true;
    }
    else if (c == '-')
    {
        lexer.setCurrentState(SubState);
        return true;
    }
    else if (c == '*')
    {
        lexer.setCurrentState(MulState);
        return true;
    }
    else if (c == '/')
    {
        lexer.setCurrentState(DivState);
        return true;
    }
    else if (c == '%')
    {
        lexer.setCurrentState(RemState);
        return true;
    }
    else if (c == '=')
    {
        lexer.setCurrentState(AssignState);
        return true;
    }
    else if (c == '!')
    {
        lexer.setCurrentState(NotState);
        return true;
    }
    else if (c == '<')
    {
        lexer.setCurrentState(LessThanState);
        return true;
    }
    else if (c == '>')
    {
        lexer.setCurrentState(LargerThanState);
        return true;
    }
    else if (c == '&')
    {
        lexer.setCurrentState(BitAndState);
        return true;
    }
    else if (c == '|')
    {
        lexer.setCurrentState(BitOrState);
        return true;
    }
    else if (IsSingleToken(c))
    {
        string val = "";
        val.push_back(c);
        lexer.addToken(Token(singleToken[c], val, row, col));
        return true;
    }
    else
    {
        string s = "";
        s.push_back(c);
        throw ByxLexer::LexError("Illegal character: " + s, row, col);
    }
}

bool IntegerState(ByxLexer& lexer, char c, int row, int col)
{
    if (isdigit(c))
    {
        lexer.addToVal(c);
        return true;
    }
    else if (c == '.')
    {
        lexer.addToVal(c);
        lexer.setCurrentState(DoubleState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Integer, lexer.getVal(), row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool DoubleState(ByxLexer& lexer, char c, int row, int col)
{
    if (isdigit(c))
    {
        lexer.addToVal(c);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Double, lexer.getVal(), row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool IdentState(ByxLexer& lexer, char c, int row, int col)
{
    if (isalnum(c) || c == '_')
    {
        lexer.addToVal(c);
        return true;
    }
    else
    {
        string val = lexer.getVal();
        if (IsKeyword(val))
        {
            lexer.addToken(Token(TokenType::Keyword, val, row, col));
        }
        else
        {
            lexer.addToken(Token(TokenType::Ident, val, row, col));
        }
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool AddState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '+')
    {
        lexer.setCurrentState(IncState);
        return true;
    }
    else if (c == '=')
    {
        lexer.setCurrentState(AddAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Add, "+", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool IncState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::Inc, "++", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool AddAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::AddAssign, "+=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool AssignState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(EqualState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Assign, "=", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool EqualState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::Equal, "==", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool SubState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '-')
    {
        lexer.setCurrentState(DecState);
        return true;
    }
    else if (c == '=')
    {
        lexer.setCurrentState(SubAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Sub, "-", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool DecState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::Dec, "--", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool SubAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::SubAssign, "-=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool MulState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(MulAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Mul, "*", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool MulAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::MulAssign, "*=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool DivState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(DivAssignState);
        return true;
    }
    else if (c == '/')
    {
        lexer.setCurrentState(LineCommentState);
        return true;
    }
    else if (c == '*')
    {
        lexer.setCurrentState(BlockCommentState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Div, "/", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool DivAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::DivAssign, "/=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool RemState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(RemAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Rem, "%", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool RemAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::RemAssign, "%=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool NotState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(NotEqualState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::Not, "!", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool NotEqualState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::NotEqual, "!=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool LessThanState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(LessEqualThanState);
        return true;
    }
    else if (c == '<')
    {
        lexer.setCurrentState(LeftShiftState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::LessThan, "<", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool LessEqualThanState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::LessEqualThan, "<=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool LargerThanState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(LargerEqualThanState);
        return true;
    }
    else if (c == '>')
    {
        lexer.setCurrentState(RightShiftState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::LargerThan, ">", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool LargerEqualThanState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::LargerEqualThan, ">=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool BitAndState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '&')
    {
        lexer.setCurrentState(AndState);
        return true;
    }
    else if (c == '=')
    {
        lexer.setCurrentState(AndAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::BitAnd, "&", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool AndState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::And, "&&", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool AndAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::AndAssign, "&=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool BitOrState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '|')
    {
        lexer.setCurrentState(OrState);
        return true;
    }
    else if (c == '=')
    {
        lexer.setCurrentState(OrAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::BitOr, "|", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool OrState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::Or, "||", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool OrAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::OrAssign, "|=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool LeftShiftState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(LeftShiftAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::LeftShift, "<<", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool LeftShiftAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::LeftShiftAssign, "<<=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool RightShiftState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '=')
    {
        lexer.setCurrentState(RightShiftAssignState);
        return true;
    }
    else
    {
        lexer.addToken(Token(TokenType::RightShift, ">>", row, col));
        lexer.setCurrentState(BeginState);
        return false;
    }
}

bool RightShiftAssignState(ByxLexer& lexer, char c, int row, int col)
{
    lexer.addToken(Token(TokenType::RightShiftAssign, ">>=", row, col));
    lexer.setCurrentState(BeginState);
    return false;
}

bool LineCommentState(ByxLexer& lexer, char c, int row, int col)
{
    if (c == '\n' || c == 0)
    {
        lexer.setCurrentState(BeginState);
        return true;
    }
    else
    {
        return true;
    }
}

bool BlockCommentState(ByxLexer& lexer, char c, int row, int col)
{
    static bool flag = false;

    if (flag)
    {
        flag = false;
        if (c == '/' || c == 0)
        {
            lexer.setCurrentState(BeginState);
            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (c == '*')
        {
            flag = true;
            return true;
        }
        else
        {
            return true;
        }
    }
}
