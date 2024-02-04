#include "Texture2D.h"

#include "stb_image.h"

#include <iostream>


Texture2D::Texture2D(const std::string& fileName, const std::string& type) :
    m_path(fileName),
    m_type(type)
{
    ID = LoadTexture();
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::DeActivate()
{
    glActiveTexture(GL_TEXTURE0);
}

void Texture2D::Activate(int idx)
{
    glActiveTexture(GL_TEXTURE0 + idx);
}

void Texture2D::Enable()
{
    glEnable(GL_TEXTURE_2D);
}

void Texture2D::Disable()
{
    glDisable(GL_TEXTURE_2D);
}

void Texture2D::SetParameterInt(GLenum name, GLint param)
{
    glTexParameteri(GL_TEXTURE_2D, name, param);
}

void Texture2D::Delete()
{
    glDeleteTextures(1, &ID);
}

std::string Texture2D::Type() const
{
    return m_type;
}

std::string Texture2D::Path() const
{
    return m_path;
}

void Texture2D::BindImage(unsigned int& textureID)
{
    int width, height, channels;

    unsigned char* data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);

    if (data)
    {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        SetParameterInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
        SetParameterInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
        SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
    }
}

unsigned int Texture2D::LoadTexture()
{
    unsigned int texture;
    glGenTextures(1, &texture); 
    BindImage(texture);
    return texture;
}
