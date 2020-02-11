#include "Scope.h"

void Scope::define(const std::string& name, int index)
{
	if (symbols.count(name) > 0)
	{
		throw SymbolRedefined();
	}
	symbols[name] = index;
}

int Scope::resolve(const std::string& name)
{
	if (symbols.count(name) == 0)
	{
		throw SymbolNotFound();
	}
	return symbols[name];
}
