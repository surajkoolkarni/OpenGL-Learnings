#pragma once

#include "Mesh.h"

#include <assimp/scene.h>

#include <string>


class MODEL_API Model
{
public:
    Model(const std::string& path);

    Model(const Model&) = delete;
    Model(Model&&) = default;

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = default;

    void Draw(std::shared_ptr<Shader>& shader);

private:
    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    void appendToMesh(aiMesh* mesh, const aiScene* scene);

private:
    std::unique_ptr<Mesh> m_mesh;
    std::vector<Texture2D> m_textureCache;
};
