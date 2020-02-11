#pragma once

#include "ASTVisitor.h"

#include <map>
#include <string>

// ɨ��ȫ�ֱ����ͺ���
class GlobalSymbolVisitor : public ASTVisitor
{
public:
	GlobalSymbolVisitor();
	std::map<std::string, int> getGlobalVarIndex() const;
	std::map<std::string, int> getFuncIndex() const;
	std::map<std::string, int> getFuncParamCount() const;

private:
	std::map<std::string, int> globalVarIndex; // ȫ�ֱ�������
	std::map<std::string, int> funcIndex; // ��������
	std::map<std::string, int> funcParamCnt; // ������������
	int numGlobalVar;
	int numFunc;

	virtual void visit(ProgramNode& node) override;
	virtual void visit(VarDeclareNode& node) override;
	virtual void visit(FunctionDeclareNode& node) override;
};