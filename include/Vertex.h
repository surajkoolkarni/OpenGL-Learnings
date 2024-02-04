#pragma once

#include "DllExport.h"

#include <glm/glm.hpp>

struct MODEL_API Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    
    int BoneIDs[4];

    float Weights[4];

    static unsigned int Length();

    static unsigned int PositionSize();

    static unsigned int NormalSize();

    static unsigned int TexSize();

    static unsigned int TangentSize();

    static unsigned int BitTangentSize();
};
