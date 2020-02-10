#pragma once

#include "AST.h"

class ASTVisitor
{
public:
	void visit(ProgramNode& node);
	void visit(EmptyNode& node);
	void visit(IntegerNode& node);
	void visit(DoubleNode& node);
	void visit(VarDeclareNode& node);
	void visit(FunctionDeclareNode& node);
	void visit(VarNode& node);
	void visit(VarAssignNode& node);
	void visit(ReturnNode& node);
	void visit(FunctionCallStmtNode& node);
	void visit(FunctionCallExprNode& node);
	void visit(IfNode& node);
	void visit(BinaryOpNode& node);
	void visit(UnaryOpNode& node);
	void visit(WhileNode& node);
	void visit(BreakNode& node);
	void visit(ContinueNode& node);
	void visit(ForNode& node);
	void visit(CodeBlockNode& node);
};