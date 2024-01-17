#pragma once

#include <string>


class StbImage
{
public:
    StbImage(const std::string& fName);

    void Free();

public:
    int width{}, height{}, channels{};

    unsigned char* data = { nullptr };

    std::string m_fName;
};
