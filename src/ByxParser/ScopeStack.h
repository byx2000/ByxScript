#pragma once

#include "Scope.h"

#include <vector>

class ScopeStack
{
public:
	ScopeStack();
	void push(); // 压入作用域栈
	void pop(); // 弹出作用域栈
	int define(const std::string& name);
	int resolve(const std::string& name);
	int getSymbolCount();

	// 异常
	class EmptyWhenPop {};
	class EmptyWhenDefine {};

private:
	std::vector<Scope> scopes;
	int symbolCount;
};