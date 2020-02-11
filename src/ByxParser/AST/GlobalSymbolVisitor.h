#pragma once

#include "ASTVisitor.h"
#include "../../Common/Module.h"

#include <map>
#include <string>

// ɨ��ȫ�ֱ����ͺ���
class GlobalSymbolVisitor : public ASTVisitor
{
public:
	GlobalSymbolVisitor();
	std::map<std::string, GlobalVarInfo> getGlobalVarInfo() const;
	std::map<std::string, FunctionInfo> getFunctionInfo() const;

private:
	int varIndex;
	int funcIndex;
	std::map<std::string, GlobalVarInfo> globalVarInfo;
	std::map<std::string, FunctionInfo> functionInfo;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
};