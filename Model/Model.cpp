#include "Model.h"

#include "Mesh.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
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

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        appendToMesh(mesh, scene);
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
