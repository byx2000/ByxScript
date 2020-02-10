#pragma once

#include "DataType.h"

class Value
{
public:
	Value();
	Value(int val);
	Value(double val);
	DataType getType() const; // 获取值类型
	Value toInt() const; // 强制转换成整型
	Value toDouble() const; // 强制转换成浮点型
	std::string toString() const;

	// 运算符重载
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

	// 异常
	class ReadUnknownDataType {}; // 读取未知类型值
	class OperateWithUnknownDataType {}; // 未知类型值参与计算

private:
	DataType type;

	union
	{
		int iVal;
		double dVal;
	};
};