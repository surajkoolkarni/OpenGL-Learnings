#pragma once

#ifdef _WIN32
	#define MODEL_API __declspec(dllexport)
#else
	#define MODEL_API 
#endif
