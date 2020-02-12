#pragma once

#include "AST.h"

class ASTVisitor
{
public:
	virtual void visit(ProgramNode& node);
	virtual void visit(EmptyNode& node);
	virtual void visit(IntegerNode& node);
	virtual void visit(DoubleNode& node);
	virtual void visit(GlobalVarDeclareNode& node);
	virtual void visit(LocalVarDeclareNode& node);
	virtual void visit(FunctionDeclareNode& node);
	virtual void visit(VarNode& node);
	virtual void visit(VarAssignNode& node);
	virtual void visit(ReturnNode& node);
	virtual void visit(FunctionCallStmtNode& node);
	virtual void visit(FunctionCallExprNode& node);
	virtual void visit(IfNode& node);
	virtual void visit(BinaryOpNode& node);
	virtual void visit(UnaryOpNode& node);
	virtual void visit(WhileNode& node);
	virtual void visit(BreakNode& node);
	virtual void visit(ContinueNode& node);
	virtual void visit(ForNode& node);
	virtual void visit(CodeBlockNode& node);
};