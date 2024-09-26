#pragma once

#include "DllExport.h"
#include "IBuffer.h"

#include <vector>

class MODEL_API ElementBuffer final : public IBuffer
{
public:
    ElementBuffer();

    ~ElementBuffer() {}

    void Copy(const void* buf, unsigned int size) override;

    void CopyIndices(const std::vector<unsigned int>& indices);
};
