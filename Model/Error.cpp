#include "Error.h"

#include <iostream>

void ClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool LogCall(const char* function, const char* file, int line)
{
	while (GLenum err = glGetError())
	{
		std::cout << "OpenGL Error:" << err << ":" << function << ":" << 
			file << ":" << line << std::endl;
		
		return false;
	}

	return true;
}
