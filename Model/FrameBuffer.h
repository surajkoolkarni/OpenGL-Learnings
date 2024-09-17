#pragma once

#include "DllExport.h"


class MODEL_API FrameBuffer
{
public:
	FrameBuffer();

	~FrameBuffer();

	void Bind();

	void UnBind();

	void AttachTexture(unsigned int id);

	void AttachRenderBuffer(unsigned int id);

	unsigned int ID;
};
