#include "Model.h"

#include "Mesh.h"
#include "GLWindow.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>

#include <iostream>


Model::Model(const std::string& path)
{
    m_mesh = std::make_unique<Mesh>();
    loadModel(path);
    m_mesh->CopyData();
}

void Model::Draw(std::shared_ptr<Shader>& shader)
{
    m_mesh->Draw(shader);
}

glm::vec3 Model::Center() const
{
    return 0.5f * (m_mesh->BoundingBox()->max + m_mesh->BoundingBox()->min);
}

std::shared_ptr<BBox> Model::BoundingBox() const
{
    return m_mesh->BoundingBox();
}

void Model::ExportToGLTF(const std::string & file)
{
}

void Model::AttachCamera(const std::string& name, const std::shared_ptr<Camera>& camera)
{
    if (!camera)
        return;

    float r = BoundingBox()->BoundingRadius();

    float theta;

    if (GLWindow::GetInstance().AspectRatio() >= 1.0)
    {
        theta = 2.0f * glm::atan(glm::tan(glm::radians(camera->Zoom) * 0.5f) / GLWindow::GetInstance().AspectRatio());
    }
    else
    {
        theta = 2.0f * glm::atan(glm::tan(glm::radians(camera->Zoom) * 0.5f) * GLWindow::GetInstance().AspectRatio());
    }

    float dist = r / glm::sin(theta * 0.5f);

    camera->FocalPoint = Center();

    camera->Front = glm::vec3(0.f, 0.f, 1.f);
    camera->Position = camera->FocalPoint + dist * camera->Front;
    camera->Up = glm::vec3(0.f, 1.f, 0.f);

    // calculate extended bounding box
    camera->NearPlane = glm::max(dist - r, 0.1f);
    camera->FarPlane = dist + r;

    m_cameras[name] = camera;
}

std::shared_ptr<Camera> Model::ToggleCamera(const std::string& name)
{
    if (m_cameras.find(name) != m_cameras.end())
        return m_cameras[name];

    return nullptr;
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    auto* scene = importer.ReadFile(path, aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);

    //std::shared_ptr<Assimp::Exporter> exporter = std::make_shared<Assimp::Exporter>();
    //exporter->Export(scene, "gltf2", "backpack.gltf");
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        appendToMesh(mesh, scene);
        m_mesh->SetBoundingBox(mesh->mAABB);
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::appendToMesh(aiMesh* mesh, const aiScene* scene)
{
    static uint32_t offset = 0;
    m_mesh->AppendVertices(mesh);
    m_mesh->AppendIndices(mesh, offset);
    m_mesh->AppendTextures(mesh, scene);
    
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        offset += face.mNumIndices;
    }
}
