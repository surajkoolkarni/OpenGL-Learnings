#include "RenderBuffer.h"

#include "GLWindow.h"

#include <glad/glad.h>

RenderBuffer::RenderBuffer()
{
	glGenRenderbuffers(1, &ID);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GLWindow::GetInstance().Width(), GLWindow::GetInstance().Height()); // use a single renderbuffer object for both a depth AND stencil buffer.
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &ID);
}

void RenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RenderBuffer::UnBind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
