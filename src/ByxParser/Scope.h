#pragma once

#include <string>
#include <map>

class Scope
{
public:
	void define(const std::string& name, int index);
	int resolve(const std::string& name);

	// �쳣
	class SymbolNotFound {}; // ����δ����
	class SymbolRedefined {}; // �����ض���

private:
	std::map<std::string, int> symbols; // �洢���ŵ�����
};