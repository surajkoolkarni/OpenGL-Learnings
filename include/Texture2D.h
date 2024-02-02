#pragma once

#include "StbImage.h"

#include <glad/glad.h>

#include <memory>


class Texture2D
{
public:
    Texture2D() = default;

    Texture2D(const std::string& fileName, const std::string& type = "");

    Texture2D(const Texture2D&) = default;
    Texture2D& operator=(const Texture2D&) = default;

    Texture2D(Texture2D&&) = default;
    Texture2D& operator=(Texture2D&&) = default;

    void Bind() const;

    static void UnBind();

    static void DeActivate();

    static void Activate(int idx);

    static void Enable();

    static void Disable();

    void SetParameterInt(GLenum name, GLint param);

    void Delete();

    std::string Type() const;

    std::string Path() const;

private:
    void BindImage(u_int& textureID);

    uint LoadTexture();

public:
    std::string m_path;
    std::string m_type;

    unsigned int ID;
};
