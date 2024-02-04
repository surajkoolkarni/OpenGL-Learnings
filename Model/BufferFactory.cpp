#include "BufferFactory.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"


std::shared_ptr<IBuffer> BufferFactory::CreateBuffer(const std::string& name)
{
    if (name == "Vertex")
        return std::make_shared<VertexBuffer>();
    else if (name == "Element")
        return std::make_shared<ElementBuffer>();
    else
        return nullptr;
}
