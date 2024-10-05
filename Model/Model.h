#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <assimp/scene.h>

#include <string>
#include <map>


class MODEL_API Model
{
public:
    Model(const std::string& path);

    Model(const Model&) = delete;
    Model(Model&&) = default;

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = default;

    void Draw(std::shared_ptr<Shader>& shader);

    void AttachCamera(const std::string& name, const std::shared_ptr<Camera>& camera);

    std::shared_ptr<Camera> ToggleCamera(const std::string& name);

    std::shared_ptr<BBox> BoundingBox() const;

    void ExportToGLTF(const std::string& file);

private:
    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    void appendToMesh(aiMesh* mesh, const aiScene* scene);

    glm::vec3 Center() const;

private:
    std::unique_ptr<Mesh> m_mesh;
    std::vector<Texture2D> m_textureCache;

    std::map<std::string, std::shared_ptr<Camera>> m_cameras;
};
