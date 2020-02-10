#include "Opcode.h"

using namespace std;

static string Symbols[] =
{
	"nop",
	"halt",
	"iconst",
	"dconst",
	"load",
	"gload",
	"store",
	"gstore",
	"add",
	"sub",
	"mul",
	"div",
	"rem",
	"equ",
	"neq",
	"g",
	"l",
	"ge",
	"le",
	"neg",
	"jmp",
	"jl",
	"jle",
	"jg",
	"jge",
	"je",
	"jne",
	"call",
	"ret",
	"toi",
	"tod",
	"pop",
	"land",
	"lor",
	"lnot",
};

std::string OpcodeToString(Opcode op)
{
	return Symbols[(int)op];
}
