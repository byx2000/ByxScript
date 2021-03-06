#pragma once

#include "CodeSeg.h"
#include "FunctionTable.h"

#include <map>
#include <string>

// 全局变量信息
struct GlobalVarInfo
{
	int index;
	bool isExport;
	GlobalVarInfo();
	GlobalVarInfo(int index, bool isExport);
	std::string toString() const;
};

// 函数信息
struct FunctionInfo
{
	int index;
	bool isExport;
	bool hasRetVal;
	int paramCount;
	int space;
	int addr;
	FunctionInfo();
	FunctionInfo(int index, bool isExport, bool hasRetVal, int paramCount, int space = -1, int addr = -1);
	std::string toString() const;
};

// 重定位条目
enum class RelocType { Var, Function };
struct RelocEntry
{
	int addr;
	RelocType type;
	std::string name;
	int paramCount;
	RelocEntry(int addr, const std::string& varName);
	RelocEntry(int addr, const std::string& funcName, int paramCount);
	std::string toString() const;
};

class Module
{
public:
private:
	CodeSeg code; // 函数代码
	CodeSeg globalCode; // 全局变量初始化代码
	std::map<std::string, GlobalVarInfo> globalVarInfo; // 全局变量信息
	std::map<std::string, FunctionInfo> functionInfo; // 函数信息
};