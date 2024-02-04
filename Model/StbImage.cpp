#include "StbImage.h"

#include "stb_image.h"

#include <iostream>

StbImage::StbImage(const std::string& fName) :
    m_fName(fName)
{
    data = stbi_load(m_fName.c_str(), &width, &height, &channels, 0);
    stbi_set_flip_vertically_on_load(true);
}

void StbImage::Free()
{
    stbi_image_free(data);
}
