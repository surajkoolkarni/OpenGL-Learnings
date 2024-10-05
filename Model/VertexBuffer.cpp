#include "VertexBuffer.h"

#include <glad/glad.h>

#include <vector>


VertexBuffer::VertexBuffer() :
    IBuffer(GL_ARRAY_BUFFER)
{
}

void VertexBuffer::Copy(const void* buf, unsigned int size)
{
    IBuffer::Bind();
    glBufferData(m_target, size * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(m_target, size * sizeof(float), buf, GL_DYNAMIC_DRAW);
}

void VertexBuffer::CopyVertices(const std::vector<Vertex>& vertices)
{
    IBuffer::Bind();
    glBufferData(m_target, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}
