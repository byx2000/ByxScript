#pragma once

#include "ASTVisitor.h"
#include "../ScopeStack.h"

class ByxParser;

class LocalSymbolVisitor : public ASTVisitor
{
public:
	LocalSymbolVisitor(ByxParser& parser);

private:
	ByxParser& parser;
	bool inGlobalScope; // 标记是否处于全局作用域
	ScopeStack scopeStack; // 局部作用域栈

	virtual void visit(ProgramNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(CodeBlockNode& node) override;
	virtual void visit(VarNode& node) override;
	virtual void visit(VarAssignNode& node) override;
	virtual void visit(ReturnNode& node) override;
	virtual void visit(FunctionCallStmtNode& node) override;
	virtual void visit(FunctionCallExprNode& node) override;
	virtual void visit(IfNode& node) override;
	virtual void visit(BinaryOpNode& node) override;
	virtual void visit(WhileNode& node) override;
	virtual void visit(UnaryOpNode& node) override;
	virtual void visit(ForNode& node) override;
};