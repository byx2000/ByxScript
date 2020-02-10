#pragma once

#include <string>

enum class DataType
{
	Integer,
	Double,
	Void,
	Unknown,
};

std::string DataTypeToString(DataType type);