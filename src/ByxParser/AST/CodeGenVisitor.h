#pragma once

#include "ASTVisitor.h"
#include "../../Common/CodeSeg.h"
#include "../../Common/Module.h"

class ByxParser;

class CodeGenVisitor : public ASTVisitor
{
public:
	CodeGenVisitor(ByxParser& parser, bool globalCode = false);
	CodeSeg getCode() const;
	std::vector<RelocEntry> getRelocTable() const;

private:
	ByxParser& parser;
	CodeSeg codeSeg;
	std::vector<RelocEntry> relocTable;

	bool globalCode;
	bool inGlobal;
	bool inLoop;
	std::vector<int> breakStmtIndex;
	std::vector<int> continueStmtIndex;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
	virtual void visit(IntegerNode& node) override;
	virtual void visit(DoubleNode& node) override;
	virtual void visit(VarNode& node) override;
	virtual void visit(VarAssignNode& node) override;
	virtual void visit(BinaryOpNode& node) override;
	virtual void visit(UnaryOpNode& node) override;
	virtual void visit(ReturnNode& node) override;
	virtual void visit(FunctionCallStmtNode& node) override;
	virtual void visit(FunctionCallExprNode& node) override;
	virtual void visit(IfNode& node) override;
	virtual void visit(WhileNode& node) override;
	virtual void visit(ForNode& node) override;
	virtual void visit(BreakNode& node) override;
	virtual void visit(ContinueNode& node) override;
	virtual void visit(CodeBlockNode& node) override;
};