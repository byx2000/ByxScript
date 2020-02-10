#pragma once

#include <string>
#include <vector>

class FunctionTable
{
public:

	// 函数表条目
	class Entry
	{
	public:
		Entry(int space, int addr);
		std::string toString() const;

		int space; // 函数所用的空间大小
		int addr; // 函数起始地址
	};

	FunctionTable(const std::vector<Entry>& table = std::vector<Entry>());
	void add(int space, int addr); // 添加函数
	int getSpace(int index) const; // 获取函数所用的空间大小
	int getAddr(int index) const; // 获取函数的起始地址
	const Entry& getAllInfo(int index) const; // 获取所有信息
	int getCount() const; // 获取函数个数

	void setAddr(int index, int addr); // 设置函数地址
	void setSpace(int index, int space); // 设置函数空间

	std::string toString() const;

	// 异常
	class FunctionIndexOutOfBoundary {}; // 函数下标越界

private:
	std::vector<Entry> table; // 函数信息表
	void checkIndex(int index) const; // 检查函数下标
};