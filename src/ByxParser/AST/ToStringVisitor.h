#pragma once

#include "ASTVisitor.h"

#include <string>

class ToStringVisitor : public ASTVisitor
{
public:
	std::string getString() const;

private:
	std::string str;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(IntegerNode& node) override;
	virtual void visit(DoubleNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
	virtual void visit(CodeBlockNode& node) override;
	virtual void visit(VarNode& node) override;
	virtual void visit(VarAssignNode& node) override;
	virtual void visit(ReturnNode& node) override;
	virtual void visit(FunctionCallStmtNode& node) override;
	virtual void visit(FunctionCallExprNode& node) override;
	virtual void visit(IfNode& node) override;
	virtual void visit(BinaryOpNode& node) override;
	virtual void visit(WhileNode& node) override;
	virtual void visit(BreakNode& node) override;
	virtual void visit(ContinueNode& node) override;
	virtual void visit(UnaryOpNode& node) override;
	virtual void visit(ForNode& node) override;
};