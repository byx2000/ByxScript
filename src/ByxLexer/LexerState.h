#pragma once

class ByxLexer;

typedef bool(*StateProc)(ByxLexer& lexer, char c, int row, int col);

bool BeginState(ByxLexer& lexer, char c, int row, int col); //初始状态
bool IntegerState(ByxLexer& lexer, char c, int row, int col); //整数
bool DoubleState(ByxLexer& lexer, char c, int row, int col); //实数
bool IdentState(ByxLexer& lexer, char c, int row, int col); //标识符
bool AddState(ByxLexer& lexer, char c, int row, int col); // +
bool IncState(ByxLexer& lexer, char c, int row, int col); // ++
bool AddAssignState(ByxLexer& lexer, char c, int row, int col); // +=
bool AssignState(ByxLexer& lexer, char c, int row, int col); // =
bool EqualState(ByxLexer& lexer, char c, int row, int col); // ==
bool SubState(ByxLexer& lexer, char c, int row, int col); // -
bool DecState(ByxLexer& lexer, char c, int row, int col); // --
bool SubAssignState(ByxLexer& lexer, char c, int row, int col); // -=
bool MulState(ByxLexer& lexer, char c, int row, int col); // *
bool MulAssignState(ByxLexer& lexer, char c, int row, int col); // *=
bool DivState(ByxLexer& lexer, char c, int row, int col); // /
bool DivAssignState(ByxLexer& lexer, char c, int row, int col); // /=
bool RemState(ByxLexer& lexer, char c, int row, int col); // %
bool RemAssignState(ByxLexer& lexer, char c, int row, int col); // %=
bool NotState(ByxLexer& lexer, char c, int row, int col); // !
bool NotEqualState(ByxLexer& lexer, char c, int row, int col); // !=
bool LessThanState(ByxLexer& lexer, char c, int row, int col); // <
bool LessEqualThanState(ByxLexer& lexer, char c, int row, int col); // <=
bool LargerThanState(ByxLexer& lexer, char c, int row, int col); // >
bool LargerEqualThanState(ByxLexer& lexer, char c, int row, int col); // >=
bool BitAndState(ByxLexer& lexer, char c, int row, int col); // &
bool AndState(ByxLexer& lexer, char c, int row, int col); // &&
bool AndAssignState(ByxLexer& lexer, char c, int row, int col); // &=
bool BitOrState(ByxLexer& lexer, char c, int row, int col); // |
bool OrState(ByxLexer& lexer, char c, int row, int col); // ||
bool OrAssignState(ByxLexer& lexer, char c, int row, int col); // |=
bool LeftShiftState(ByxLexer& lexer, char c, int row, int col); // <<
bool LeftShiftAssignState(ByxLexer& lexer, char c, int row, int col); // <<=
bool RightShiftState(ByxLexer& lexer, char c, int row, int col); // >>
bool RightShiftAssignState(ByxLexer& lexer, char c, int row, int col); // >>=
bool LineCommentState(ByxLexer& lexer, char c, int row, int col); // 行注释
bool BlockCommentState(ByxLexer& lexer, char c, int row, int col); // 块注释
