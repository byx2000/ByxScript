#pragma once

#include "ASTVisitor.h"
#include "../../Common/CodeSeg.h"

class ByxParser;

class GlobalCodeGenVisitor : public ASTVisitor
{
public:
	GlobalCodeGenVisitor(ByxParser& parser);
	CodeSeg getCode() const;

private:
	ByxParser& parser;
	CodeSeg code;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
};