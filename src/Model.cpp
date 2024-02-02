#include "Model.h"

#include "Mesh.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>


Model::Model(const std::string& path)
{
    loadModel(path);
}

void Model::Draw(std::shared_ptr<Shader> shader)
{
    for (unsigned int i = 0; i < m_meshes.size(); ++i)
        m_meshes[i].Draw(shader);
}

void Model::CleanUp() const
{
    for (size_t i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Delete();
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
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
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    return Mesh(loadVertices(mesh), loadIndices(mesh), loadTextures(mesh, scene));
}

u_int Model::LoadTexture(const std::string& path)
{
    uint texture;
    glGenTextures(1, &texture); 

    int width, height, channels;

    u_char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    stbi_set_flip_vertically_on_load(true);

    if (data)
    {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
    }

    return texture;
}


std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture2D> textures;

    for(unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skip = false;
        for (size_t j = 0; j < m_texturesLoaded.size(); ++j)
        {
            if (m_texturesLoaded[j].Path() == str.C_Str())
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture2D texture;
            texture.m_path = str.C_Str();
            texture.m_type = typeName;
            texture.ID = LoadTexture(texture.m_path);

            textures.push_back(texture);
            m_texturesLoaded.push_back(texture);
        }
    }

    return textures;
}

std::vector<Vertex> Model::loadVertices(aiMesh* mesh)
{
    std::vector<Vertex> vertices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        
        if (mesh->HasNormals())
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0);

        vertices.push_back(std::move(vertex));
    }

    return vertices;
}

std::vector<unsigned int> Model::loadIndices(aiMesh* mesh)
{
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);            
    }

    return indices;
}

std::vector<Texture2D> Model::loadTextures(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Texture2D> textures;

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture2D> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return textures;
}
