#pragma once

#include <iostream>

#define ASSERT(expr) \
do \
{ \
	if (!(expr)) \
	{ \
		cout << "Test failed at line " << __LINE__ << ".\a" << endl; \
		exit(0); \
	} \
} while (0) \

#define ASSERT_EXCEPTION(expr) \
do \
{ \
    bool flag = false; \
    try \
    { \
        expr; \
    } \
    catch (...)\
    { \
        flag = true; \
    }\
    if (!flag) \
	{ \
		cout << "Test failed at line " << __LINE__ << ".\a" << endl; \
		exit(0); \
	} \
} while (0) \


class StopWatch
{
public:
	StopWatch();
	~StopWatch();
	void begin();
	void end();
	double duration();
private:
	enum { STOP, RUN } status;
	void* freq;
	void* beginTime;
	void* endTime;
};

std::string FileToString(const std::string& filename);