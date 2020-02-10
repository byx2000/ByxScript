#include "Value.h"
#include "../ByxVM/ByxVM.h"

using namespace std;

Value::Value()
	: iVal(0), type(DataType::Unknown)
{

}

Value::Value(int val)
	: iVal(val), type(DataType::Integer)
{
	
}

Value::Value(double val)
	: dVal(val), type(DataType::Double)
{

}

DataType Value::getType() const
{
	return type;
}

Value Value::toInt() const
{
	if (type == DataType::Integer)
	{
		return *this;
	}
	else if (type == DataType::Double)
	{
		return Value((int)dVal);
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::toDouble() const
{
	if (type == DataType::Integer)
	{
		return Value((double)dVal);
	}
	else if (type == DataType::Double)
	{
		return *this;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

std::string Value::toString() const
{
	string s = "";
	if (type == DataType::Integer)
	{
		s += "int\t";
		s += to_string(iVal);
	}
	else if (type == DataType::Double)
	{
		s += "double\t";
		s += to_string(dVal);
	}
	else
	{
		s += "unknown";
	}
	return s;
}

Value& Value::operator+=(const Value& v)
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		iVal += v.iVal;
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		dVal = (double)iVal + v.dVal;
		type = DataType::Double;
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		dVal += (double)v.iVal;
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		dVal += v.dVal;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}

	return *this;
}

Value Value::operator+(const Value& v) const
{
	return Value(*this) += v;
}

Value& Value::operator-=(const Value& v)
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		iVal -= v.iVal;
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		dVal = (double)iVal - v.dVal;
		type = DataType::Double;
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		dVal -= (double)v.iVal;
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		dVal -= v.dVal;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}

	return *this;
}

Value Value::operator-(const Value& v) const
{
	return Value(*this) -= v;
}

Value& Value::operator*=(const Value& v)
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		iVal *= v.iVal;
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		dVal = (double)iVal * v.dVal;
		type = DataType::Double;
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		dVal *= (double)v.iVal;
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		dVal *= v.dVal;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}

	return *this;
}

Value Value::operator*(const Value& v) const
{
	return Value(*this) *= v;
}

Value& Value::operator/=(const Value& v)
{
	// ³ýÁã¼ì²â
	if ((v.type == DataType::Integer && v.iVal == 0) ||
		(v.type == DataType::Double && v.dVal == 0.0))
	{
		throw ByxVM::ByxVMError("Divided by zero.");
	}

	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		iVal /= v.iVal;
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		dVal = (double)iVal / v.dVal;
		type = DataType::Double;
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		dVal /= (double)v.iVal;
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		dVal /= v.dVal;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}

	return *this;
}

Value Value::operator/(const Value& v) const
{
	return Value(*this) /= v;
}

Value& Value::operator%=(const Value& v)
{
	// Êý¾ÝÀàÐÍ¼ì²â
	if (type != DataType::Integer || v.type != DataType::Integer)
	{
		throw ByxVM::ByxVMError("Divided by zero.");
	}

	// ³ýÁã¼ì²â
	if (v.iVal == 0)
	{
		throw ByxVM::ByxVMError("Remander by zero.");
	}

	iVal %= v.iVal;

	return *this;
}

Value Value::operator%(const Value& v) const
{
	return Value(*this) %= v;
}

Value Value::operator==(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal == v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal == v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal == v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal == v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator!=(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal != v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal != v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal != v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal != v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator>(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal > v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal > v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal > v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal > v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator<(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal < v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal < v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal < v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal < v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator>=(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal >= v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal >= v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal >= v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal >= v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator<=(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal <= v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal <= v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal <= v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal <= v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator&&(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal && v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal && v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal && v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal && v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator||(const Value& v) const
{
	if (type == DataType::Integer && v.type == DataType::Integer)
	{
		return Value((int)(iVal || v.iVal));
	}
	else if (type == DataType::Integer && v.type == DataType::Double)
	{
		return Value((int)(iVal || v.dVal));
	}
	else if (type == DataType::Double && v.type == DataType::Integer)
	{
		return Value((int)(dVal || v.iVal));
	}
	else if (type == DataType::Double && v.type == DataType::Double)
	{
		return Value((int)(dVal || v.dVal));
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator!() const
{
	if (type == DataType::Integer)
	{
		return Value((int)!iVal);
	}
	else if (type == DataType::Double)
	{
		return Value((int)!dVal);
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value Value::operator-() const
{
	if (type == DataType::Integer)
	{
		return Value(-iVal);
	}
	else if (type == DataType::Double)
	{
		return Value(-dVal);
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}

Value::operator bool() const
{
	if (type == DataType::Integer)
	{
		return (bool)iVal;
	}
	else if (type == DataType::Double)
	{
		return (bool)dVal;
	}
	else
	{
		throw OperateWithUnknownDataType();
	}
}
