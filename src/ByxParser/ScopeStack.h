#pragma once

#include "Scope.h"

#include <vector>

class ScopeStack
{
public:
	ScopeStack();
	void push(); // ѹ��������ջ
	void pop(); // ����������ջ
	int define(const std::string& name);
	int resolve(const std::string& name);
	int getSymbolCount();

	// �쳣
	class EmptyWhenPop {};
	class EmptyWhenDefine {};

private:
	std::vector<Scope> scopes;
	int symbolCount;
};