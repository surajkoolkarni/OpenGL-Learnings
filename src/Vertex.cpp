#include "Vertex.h"


unsigned int Vertex::Length()
{
    return PositionSize() + NormalSize() + TexSize();
}

unsigned int Vertex::PositionSize()
{
    return glm::vec3::length();
}

unsigned int Vertex::NormalSize()
{
    return glm::vec3::length();
}

unsigned int Vertex::TexSize()
{
    return glm::vec2::length();
}
