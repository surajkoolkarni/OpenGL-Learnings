#include "IBuffer.h"


void IBuffer::Delete()
{
    glDeleteBuffers(1, &m_bufferObj);
}

void IBuffer::Bind() const
{
    glBindBuffer(m_target, m_bufferObj);
}

IBuffer::IBuffer(GLenum target) :
    m_target(target)
{
    glGenBuffers(1, &m_bufferObj);
}
