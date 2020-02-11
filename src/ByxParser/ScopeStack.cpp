#include "ScopeStack.h"

using namespace std;

ScopeStack::ScopeStack()
{
	symbolCount = 0;
}

void ScopeStack::push()
{
	scopes.push_back(Scope());
}

void ScopeStack::pop()
{
	if (scopes.size() == 0)
	{
		throw EmptyWhenPop();
	}
	scopes.pop_back();
}

int ScopeStack::define(const std::string& name)
{
	if (scopes.size() == 0)
	{
		throw EmptyWhenDefine();
	}
	scopes[(int)scopes.size() - 1].define(name, symbolCount++);
	return symbolCount - 1;
}

int ScopeStack::resolve(const std::string& name)
{
	for (int i = (int)scopes.size() - 1; i >= 0; --i)
	{
		try
		{
			return scopes[i].resolve(name);
		}
		catch (...) {}
	}
	throw Scope::SymbolNotFound();
}

int ScopeStack::getSymbolCount()
{
	return symbolCount;
}
