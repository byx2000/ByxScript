#pragma once

#include <string>
#include <map>

class Scope
{
public:
	void define(const std::string& name, int index);
	int resolve(const std::string& name);

	// 异常
	class SymbolNotFound {}; // 符号未定义
	class SymbolRedefined {}; // 符号重定义

private:
	std::map<std::string, int> symbols; // 存储符号的索引
};