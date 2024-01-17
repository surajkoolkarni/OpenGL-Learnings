#pragma once

#include "Vertex.h"

#include <glad/glad.h>


class IBuffer
{
public:
    virtual void Copy(const void* buf, unsigned int size) = 0;

    virtual void Delete();

    virtual void Bind() const;

    virtual ~IBuffer() = default;

protected:
    IBuffer(GLenum target);

protected:
    unsigned int m_bufferObj;

    GLenum m_target;
};
