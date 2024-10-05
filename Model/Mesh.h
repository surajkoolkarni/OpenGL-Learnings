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


struct BBox
{
    glm::vec3 max;
    glm::vec3 min;

    float Width() const
    {
        return max.x - min.x;
    }

    float Height() const
    {
        return max.y - min.y;
    }

    float Depth() const
    {
        return max.z - min.z;
    }

    float MaxExtent() const
    {
        return glm::max(Width(), glm::max(Height(), Depth()));
    }

    float BoundingRadius() const
    {
        return 0.5 * glm::distance(max, min);
    }
};

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

    void SetBoundingBox(const aiAABB& bbox);

    std::shared_ptr<BBox> BoundingBox() const;

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

    std::shared_ptr<BBox> m_bbox;
};
