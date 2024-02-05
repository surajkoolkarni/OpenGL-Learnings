#include "IBuffer.h"
#include "Error.h"


void IBuffer::Bind() const
{
    GLCall(glBindBuffer(m_target, m_bufferObj));
}

void IBuffer::UnBind() const
{
    GLCall(glBindBuffer(m_target, 0));
}

IBuffer::~IBuffer()
{
    GLCall(glDeleteBuffers(1, &m_bufferObj));
}

IBuffer::IBuffer(GLenum target) :
    m_target(target)
{
    GLCall(glGenBuffers(1, &m_bufferObj));
}
