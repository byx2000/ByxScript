#include "DataType.h"

using namespace std;

static string str[] =
{
	"int",
	"double",
	"void",
	"unknown",
};

std::string DataTypeToString(DataType type)
{
	return str[(int)type];
}