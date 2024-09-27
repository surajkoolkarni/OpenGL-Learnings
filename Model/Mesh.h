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
#include <cstdint>

#include <assimp/mesh.h>
#include <assimp/scene.h>


class Mesh
{
public:
    Mesh();

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh& mesh) = delete;
    Mesh& operator=(Mesh&& mesh) = default;

    void Draw(std::shared_ptr<Shader>& shader);

    void CopyData() const;

    void AppendVertices(aiMesh* mesh);

    void AppendIndices(aiMesh* mesh, uint32_t offset);

    void AppendTextures(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture2D> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

private:
    uint32_t LoadTexture(const std::string& path);

    void setAttributes() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture2D> m_textureCache;

    std::unique_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<ElementBuffer> m_EBO;

    bool m_isSet = false;
};
