#include "VertexArray.h"

#include <glad/glad.h>


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VAO); 
}

void VertexArray::Bind() const
{
    glBindVertexArray(VAO);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void VertexArray::DrawVertexArrays(unsigned int count)
{
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void VertexArray::DrawElements(unsigned int count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void VertexArray::Delete() const
{
    glDeleteVertexArrays(1, &VAO);
}

void VertexArray::SetVertexAttributes(int idx, unsigned int size, unsigned int stride, unsigned int offset)
{
    glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(idx);
}
