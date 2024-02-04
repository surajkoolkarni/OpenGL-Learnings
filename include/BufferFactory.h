#pragma once

#include "DllExport.h"
#include "IBuffer.h"

#include <memory>
#include <string>


class MODEL_API BufferFactory
{
public:
    BufferFactory() = delete;

    BufferFactory(const BufferFactory&) = delete;

    static std::shared_ptr<IBuffer> CreateBuffer(const std::string& name);
};
