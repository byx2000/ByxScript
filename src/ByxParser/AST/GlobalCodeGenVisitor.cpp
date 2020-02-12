#include "GlobalCodeGenVisitor.h"
#include "../ByxParser.h"
#include "CodeGenVisitor.h"

using namespace std;

GlobalCodeGenVisitor::GlobalCodeGenVisitor(ByxParser& parser)
	: parser(parser)
{

}

CodeSeg GlobalCodeGenVisitor::getCode() const
{
	return code;
}

void GlobalCodeGenVisitor::visit(ProgramNode& node)
{
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
	}
}

void GlobalCodeGenVisitor::visit(GlobalVarDeclareNode& node)
{
	CodeGenVisitor visitor(parser);
	node.visit(visitor);
	code.add(visitor.getCode());
}
