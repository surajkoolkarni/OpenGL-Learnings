#include "IBuffer.h"
#include "Error.h"


void IBuffer::Delete()
{
    GLCall(glDeleteBuffers(1, &m_bufferObj));
}

void IBuffer::Bind() const
{
    GLCall(glBindBuffer(m_target, m_bufferObj));
}

IBuffer::IBuffer(GLenum target) :
    m_target(target)
{
    GLCall(glGenBuffers(1, &m_bufferObj));
}
