#include "FrameBuffer.h"

#include <glad/glad.h>


FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &ID);
	Bind();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &ID);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AttachTexture(unsigned int id)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
}

void FrameBuffer::AttachRenderBuffer(unsigned int id)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id); // now actually attach it
}
