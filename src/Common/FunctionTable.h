#pragma once

#include <string>
#include <vector>

class FunctionTable
{
public:

	// ��������Ŀ
	class Entry
	{
	public:
		Entry(int space, int addr);
		std::string toString() const;

		int space; // �������õĿռ��С
		int addr; // ������ʼ��ַ
	};

	FunctionTable(const std::vector<Entry>& table = std::vector<Entry>());
	void add(int space, int addr); // ��Ӻ���
	int getSpace(int index) const; // ��ȡ�������õĿռ��С
	int getAddr(int index) const; // ��ȡ��������ʼ��ַ
	const Entry& getAllInfo(int index) const; // ��ȡ������Ϣ
	int getCount() const; // ��ȡ��������

	void setAddr(int index, int addr); // ���ú�����ַ
	void setSpace(int index, int space); // ���ú����ռ�

	std::string toString() const;

	// �쳣
	class FunctionIndexOutOfBoundary {}; // �����±�Խ��

private:
	std::vector<Entry> table; // ������Ϣ��
	void checkIndex(int index) const; // ��麯���±�
};