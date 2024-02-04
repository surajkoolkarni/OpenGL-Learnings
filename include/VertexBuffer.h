#pragma once

#include "IBuffer.h"
#include "Vertex.h"

#include <vector>


class MODEL_API VertexBuffer final : public IBuffer
{
public:
    VertexBuffer();

    void Copy(const void* buf, unsigned int size) override;

    void CopyVertices(const std::vector<Vertex>& vertices);
};
