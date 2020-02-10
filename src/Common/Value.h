#pragma once

#include "DataType.h"

class Value
{
public:
	Value();
	Value(int val);
	Value(double val);
	DataType getType() const; // ��ȡֵ����
	Value toInt() const; // ǿ��ת��������
	Value toDouble() const; // ǿ��ת���ɸ�����
	std::string toString() const;

	// ���������
	Value& operator+=(const Value& v);
	Value operator+(const Value& v) const;
	Value& operator-=(const Value& v);
	Value operator-(const Value& v) const;
	Value& operator*=(const Value& v);
	Value operator*(const Value& v) const;
	Value& operator/=(const Value& v);
	Value operator/(const Value& v) const;
	Value& operator%=(const Value& v);
	Value operator%(const Value& v) const;
	Value operator==(const Value& v) const;
	Value operator!=(const Value& v) const;
	Value operator>(const Value& v) const;
	Value operator<(const Value& v) const;
	Value operator>=(const Value& v) const;
	Value operator<=(const Value& v) const;
	Value operator&&(const Value& v) const;
	Value operator||(const Value& v) const;
	Value operator!() const;
	Value operator-() const;
	operator bool() const;

	// �쳣
	class ReadUnknownDataType {}; // ��ȡδ֪����ֵ
	class OperateWithUnknownDataType {}; // δ֪����ֵ�������

private:
	DataType type;

	union
	{
		int iVal;
		double dVal;
	};
};