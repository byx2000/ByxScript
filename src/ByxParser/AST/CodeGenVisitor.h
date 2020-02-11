#pragma once

#include "ASTVisitor.h"
#include "../../Common/CodeSeg.h"
#include "../../Common/Module.h"

class ByxParser;

class CodeGenVisitor : public ASTVisitor
{
public:
	CodeGenVisitor(ByxParser& parser);

private:
	ByxParser& parser;
	CodeSeg codeSeg;
	std::vector<RelocEntry> relocTable;

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
	virtual void visit(BinaryOpNode& node) override;
	virtual void visit(UnaryOpNode& node) override;
};