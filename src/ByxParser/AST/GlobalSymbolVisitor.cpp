#include "GlobalSymbolVisitor.h"
#include "../ByxParser.h"

using namespace std;

GlobalSymbolVisitor::GlobalSymbolVisitor()
{
	numGlobalVar = 0;
	numFunc = 0;
}

std::map<std::string, int> GlobalSymbolVisitor::getGlobalVarIndex() const
{
	return globalVarIndex;
}

std::map<std::string, int> GlobalSymbolVisitor::getFuncIndex() const
{
	return funcIndex;
}

std::map<std::string, int> GlobalSymbolVisitor::getFuncParamCount() const
{
	return funcParamCnt;
}

void GlobalSymbolVisitor::visit(ProgramNode& node)
{
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
	}
}

void GlobalSymbolVisitor::visit(VarDeclareNode& node)
{
	// 全局变量重复定义
	if (globalVarIndex.count(node.name) > 0)
	{
		throw ByxParser::ParseError(string("Global var '") + node.name + "' is redefined.", node.row(), node.col());
	}
	globalVarIndex[node.name] = numGlobalVar++;
}

void GlobalSymbolVisitor::visit(FunctionDeclareNode& node)
{
	// 函数重复定义
	if (funcIndex.count(node.name) > 0)
	{
		throw ByxParser::ParseError(string("Function '") + node.name + "' is redefined.", node.row(), node.col());
	}
	funcIndex[node.name] = numFunc++;
	funcParamCnt[node.name] = node.paramName.size();
}
