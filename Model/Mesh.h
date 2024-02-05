#pragma once

#include "DllExport.h"
#include "Vertex.h"
#include "Texture2D.h"

#include "IBuffer.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "BufferFactory.h"
#include "VertexArray.h"
#include "Shader.h"

#include <vector>


class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh& mesh) = delete;
    Mesh& operator=(Mesh&& mesh) = default;

    void Draw(std::shared_ptr<Shader> shader) const;

private:
    void setupMesh();

    void initVAO();

    void setAttributes();

    void setupVBO();

    void setupEBO();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture2D> m_textures;

    std::unique_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<ElementBuffer> m_EBO;
};
