#include "Module.h"

using namespace std;

GlobalVarInfo::GlobalVarInfo()
{
	index = -1;
	isExport = false;
}

GlobalVarInfo::GlobalVarInfo(int index, bool isExport)
	: index(index), isExport(isExport)
{

}

std::string GlobalVarInfo::toString() const
{
	string s = "index=";
	s += to_string(index);
	s += ", isExport=";
	s += to_string(isExport);
	return s;
}

FunctionInfo::FunctionInfo()
{
	index = -1;
	isExport = false;
	paramCount = -1;
	space = -1;
	addr = -1;
}

FunctionInfo::FunctionInfo(int index, bool isExport, int paramCount, int space, int addr)
	: index(index), isExport(isExport), paramCount(paramCount), space(space), addr(addr)
{

}

std::string FunctionInfo::toString() const
{
	string s = "index=";
	s += to_string(index);
	s += ", isExport=";
	s += to_string(isExport);
	s += ", paramCount=";
	s += to_string(paramCount);
	s += ", space=";
	s += to_string(space);
	s += ", addr=";
	s += to_string(addr);
	return s;
}

RelocEntry::RelocEntry(int addr, const std::string& varName)
	: addr(addr), type(RelocType::Var), name(varName), paramCount(-1)
{
	
}

RelocEntry::RelocEntry(int addr, const std::string& funcName, int paramCount)
	: addr(addr), type(RelocType::Function), name(funcName), paramCount(paramCount)
{

}

std::string RelocEntry::toString() const
{
	string s = "addr=";
	s += to_string(addr);
	s += " type=";
	if (type == RelocType::Var)
	{
		s += "var";
	}
	else
	{
		s += "func";
	}
	s += " name=";
	s += name;
	if (type == RelocType::Function)
	{
		s += " paramCount=";
		s += to_string(paramCount);
	}
	return s;
}
