#pragma once

#include "DllExport.h"
#include "Vertex.h"

#include <glad/glad.h>


class IBuffer
{
public:
    virtual void Copy(const void* buf, unsigned int size) = 0;

    virtual void Bind() const;

    virtual void UnBind() const;

    virtual ~IBuffer();

protected:
    IBuffer(GLenum target);

protected:
    unsigned int m_bufferObj;

    GLenum m_target;
};
