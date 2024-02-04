#pragma once

#include <glad/glad.h>

#ifndef _WIN32
#include <signal.h>
#endif

#ifdef _WIN32
	#define ASSERT(x) if (!(x)) __debugbreak();
#else
	#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#endif

#define GLCall(x) ClearError();\
	x;\
	ASSERT(LogCall(#x, __FILE__, __LINE__));

void ClearError();

bool LogCall(const char* function, const char* file, int line);