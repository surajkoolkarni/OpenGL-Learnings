#pragma once

#include "IBuffer.h"

#include <memory>
#include <string>


class BufferFactory
{
public:
    BufferFactory() = delete;

    BufferFactory(const BufferFactory&) = delete;

    static std::shared_ptr<IBuffer> CreateBuffer(const std::string& name);
};
