#pragma once

#include "DllExport.h"

class MODEL_API RenderBuffer
{
public:
	RenderBuffer();

	~RenderBuffer();

	void Bind();

	void UnBind();

	unsigned int ID;
};