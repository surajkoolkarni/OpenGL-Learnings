#include "ElementBuffer.h"

#include <glad/glad.h>


ElementBuffer::ElementBuffer() :
    IBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
}

void ElementBuffer::Copy(const void* buf, unsigned int size)
{
    IBuffer::Bind();
    glBufferData(m_target, size * sizeof(unsigned int), buf, GL_STATIC_DRAW);        
}

void ElementBuffer::CopyIndices(const std::vector<unsigned int>& indices)
{
    Copy(indices.data(), indices.size());
}
