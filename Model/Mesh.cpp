#include "Mesh.h"

#include "Vertex.h"
#include "Texture2D.h"

#include "IBuffer.h"
#include "BufferFactory.h"
#include "VertexArray.h"
#include "Shader.h"

#include <vector>
#include <iostream>
#include <cstddef>


Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture2D>& textures):
    m_vertices(vertices),
    m_indices(indices),
    m_textures(textures)
{
    setupMesh();
}

void Mesh::Draw(std::shared_ptr<Shader> shader) const
{
    unsigned int diffuseIdx = 1;
    unsigned int specularIdx = 1;
    unsigned int normalIdx = 1;
    unsigned int heightIdx = 1;
    
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        Texture2D::Activate(i);
        
        m_textures[i].Bind();

        Texture2D::Enable();

        std::string number;
        std::string name = m_textures[i].Type();

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

void Mesh::setupMesh()
{
    m_VAO = std::make_unique<VertexArray>();        
    m_VBO = std::dynamic_pointer_cast<VertexBuffer>(BufferFactory::CreateBuffer("Vertex"));
    m_EBO = std::dynamic_pointer_cast<ElementBuffer>(BufferFactory::CreateBuffer("Element"));

    m_VAO->Bind();
    m_VBO->CopyVertices(m_vertices);
    m_EBO->CopyIndices(m_indices);

    setAttributes();

    m_VAO->UnBind();
}

void Mesh::setAttributes()
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
