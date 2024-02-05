#pragma once

#include "DllExport.h"

#include <glad/glad.h>


class MODEL_API VertexArray
{
public:
    VertexArray();

    ~VertexArray();

    void Bind() const;

    void UnBind() const;

    void DrawVertexArrays(unsigned int count);

    void DrawElements(unsigned int count);

    void SetVertexAttributes(int idx, unsigned int size, unsigned int stride, unsigned int offset);

private:
    unsigned int VAO;
};
