#pragma once

#include "ASTVisitor.h"

#include <map>
#include <string>

// 扫描全局变量和函数
class GlobalSymbolVisitor : public ASTVisitor
{
public:
	GlobalSymbolVisitor();
	std::map<std::string, int> getGlobalVarIndex() const;
	std::map<std::string, int> getFuncIndex() const;
	std::map<std::string, int> getFuncParamCount() const;

private:
	std::map<std::string, int> globalVarIndex; // 全局变量索引
	std::map<std::string, int> funcIndex; // 函数索引
	std::map<std::string, int> funcParamCnt; // 函数参数数量
	int numGlobalVar;
	int numFunc;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
};