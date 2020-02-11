#pragma once

#include "CodeSeg.h"
#include "FunctionTable.h"

#include <map>
#include <string>

// ȫ�ֱ�����Ϣ
struct GlobalVarInfo
{
	int index;
	bool isExport;
	GlobalVarInfo();
	GlobalVarInfo(int index, bool isExport);
	std::string toString() const;
};

// ������Ϣ
struct FunctionInfo
{
	int index;
	bool isExport;
	int paramCount;
	int space;
	int addr;
	FunctionInfo();
	FunctionInfo(int index, bool isExport, int paramCount, int space = -1, int addr = -1);
	std::string toString() const;
};

class Module
{
public:
private:
	CodeSeg code; // ��������
	CodeSeg globalCode; // ȫ�ֱ�����ʼ������
	std::map<std::string, GlobalVarInfo> globalVarInfo; // ȫ�ֱ�����Ϣ
	std::map<std::string, FunctionInfo> functionInfo; // ������Ϣ
};