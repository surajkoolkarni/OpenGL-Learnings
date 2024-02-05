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

    void Draw(std::shared_ptr<Shader> shader);

private:
    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture2D> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

    std::vector<Vertex> loadVertices(aiMesh* mesh);

    std::vector<unsigned int> loadIndices(aiMesh* mesh);

    std::vector<Texture2D> loadTextures(aiMesh* mesh, const aiScene* scene);

    unsigned int LoadTexture(const std::string& path);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture2D> m_texturesLoaded;
};
