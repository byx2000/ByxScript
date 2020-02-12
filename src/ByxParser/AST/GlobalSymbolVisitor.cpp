#include "GlobalSymbolVisitor.h"
#include "../ByxParser.h"

#include <iostream>

using namespace std;

GlobalSymbolVisitor::GlobalSymbolVisitor()
{
	varIndex = 0;
	funcIndex = 0;
}

std::map<std::string, GlobalVarInfo> GlobalSymbolVisitor::getGlobalVarInfo() const
{
	return globalVarInfo;
}

std::map<std::string, FunctionInfo> GlobalSymbolVisitor::getFunctionInfo() const
{
	return functionInfo;
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
	if (globalVarInfo.count(node.name) > 0)
	{
		throw ByxParser::ParseError(string("Global var '") + node.name + "' is redefined.", node.row(), node.col());
	}
	globalVarInfo[node.name] = GlobalVarInfo(varIndex++, node.isExport);
	node.index = varIndex - 1;
	node.isGlobal = true;
	cout << "find a global var: " << node.name << endl;
}

void GlobalSymbolVisitor::visit(FunctionDeclareNode& node)
{
	// 函数重复定义
	if (functionInfo.count(node.name) > 0)
	{
		throw ByxParser::ParseError(string("Function '") + node.name + "' is redefined.", node.row(), node.col());
	}
	functionInfo[node.name] = FunctionInfo(funcIndex++, node.isExport, node.paramName.size());
}
