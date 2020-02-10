#include "TestUtil.h"

#include <Windows.h>
#include <fstream>
#include <sstream>

using namespace std;

StopWatch::StopWatch()
{
	freq = new LARGE_INTEGER();
	beginTime = new LARGE_INTEGER();
	endTime = new LARGE_INTEGER();
	status = STOP;
	QueryPerformanceFrequency((LARGE_INTEGER*)freq);
	memset(beginTime, 0, sizeof(LARGE_INTEGER));
	memset(endTime, 0, sizeof(LARGE_INTEGER));
}

StopWatch::~StopWatch()
{
	delete freq;
	delete beginTime;
	delete endTime;
}

void StopWatch::begin()
{
	if (status == STOP)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)beginTime);
		status = RUN;
	}
}

void StopWatch::end()
{
	if (status == RUN)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)endTime);
		status = STOP;
	}
}

double StopWatch::duration()
{
	if (status == STOP)
	{
		return (double)(((LARGE_INTEGER*)endTime)->QuadPart - ((LARGE_INTEGER*)beginTime)->QuadPart) / (double)((LARGE_INTEGER*)freq)->QuadPart;
	}
	else
	{
		return -1;
	}
}

std::string FileToString(const std::string& filename)
{
	ifstream fin(filename);
	if (!fin)
	{
		cout << "Failed to open file: " << filename << endl;
		exit(0);
	}

	stringstream buffer;
	buffer << fin.rdbuf();
	return buffer.str();
}
