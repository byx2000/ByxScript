#include <iostream>
#include "ByxLexer/Test/ByxLexerTest.h"

using namespace std;

int main()
{
	// ÄÚ´æÐ¹Â©¼ì²â
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ByxLexer²âÊÔ
	ByxLexerTest::Run();

	return 0;
}