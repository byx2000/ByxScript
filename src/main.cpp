#include <iostream>
#include "ByxLexer/Test/ByxLexerTest.h"

using namespace std;

int main()
{
	// �ڴ�й©���
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ByxLexer����
	ByxLexerTest::Run();

	return 0;
}