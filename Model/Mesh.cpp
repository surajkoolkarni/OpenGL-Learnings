#include "Mesh.h"

#include "Vertex.h"
#include "Texture2D.h"

#include "IBuffer.h"
#include "BufferFactory.h"
#include "VertexArray.h"
#include "Shader.h"

#include "stb_image.h"

#include <vector>
#include <iostream>
#include <cstddef>


Mesh::Mesh()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::dynamic_pointer_cast<VertexBuffer>(BufferFactory::CreateBuffer("Vertex"));
    m_EBO = std::dynamic_pointer_cast<ElementBuffer>(BufferFactory::CreateBuffer("Element"));
}

void Mesh::Draw(std::shared_ptr<Shader>& shader)
{
    unsigned int diffuseIdx = 1;
    unsigned int specularIdx = 1;
    unsigned int normalIdx = 1;
    unsigned int heightIdx = 1;
    
    for (unsigned int i = 0; i < m_textureCache.size(); ++i)
    {
        Texture2D::Activate(i);
        
        m_textureCache[i].Bind();

        Texture2D::Enable();

        std::string number;
        std::string name = m_textureCache[i].Type();

        if(name == "texture_diffuse")
            number = std::to_string(diffuseIdx++);
        else if(name == "texture_specular")
            number = std::to_string(specularIdx++);
        else if(name == "texture_normal")
            number = std::to_string(normalIdx++);
         else if(name == "texture_height")
            number = std::to_string(heightIdx++);
 
        shader->setInt(name + number, i);

        Texture2D::Disable();
    }

    m_VAO->Bind();
    m_VAO->DrawElements(m_indices.size());
    m_VAO->UnBind();

    Texture2D::UnBind();
    Texture2D::DeActivate();
}

void Mesh::CopyData() const
{
    m_VAO->Bind();
    m_VBO->CopyVertices(m_vertices);
    m_EBO->CopyIndices(m_indices);

    setAttributes();

    m_VAO->UnBind();
}

void Mesh::AppendVertices(aiMesh* mesh)
{
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

        m_vertices.push_back(std::move(vertex));
    }
}

void Mesh::AppendIndices(aiMesh* mesh, uint32_t offset)
{
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; j++)
            m_indices.push_back(face.mIndices[j] + offset);            
    }
}

void Mesh::AppendTextures(aiMesh* mesh, const aiScene* scene)
{
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    m_textureCache.insert(m_textureCache.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    m_textureCache.insert(m_textureCache.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    m_textureCache.insert(m_textureCache.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture2D> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    m_textureCache.insert(m_textureCache.end(), heightMaps.begin(), heightMaps.end());
}

uint32_t Mesh::LoadTexture(const std::string& path)
{
    unsigned int texture;
    glGenTextures(1, &texture); 

    int width, height, channels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
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

std::vector<Texture2D> Mesh::loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture2D> textures;

    for(unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skip = false;
        for (size_t j = 0; j < m_textureCache.size(); ++j)
        {
            if (m_textureCache[j].Path() == str.C_Str())
            {
                textures.push_back(m_textureCache[j]);
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
            m_textureCache.push_back(texture);
        }
    }

    return textures;
}

void Mesh::setAttributes() const
{
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));
}
