//#include "LoadTexture.h"
//#include<stb/std_image.h>

/*
using namespace std;

void LoadTexture::loadTexture(unsigned& textureIdex, const string& fileName)
{
    int width, height, channelsNumber;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channelsNumber, 0);
    if (!data)
    {
        stbi_image_free(data);
        throw runtime_error(string("Failed to load texture from file: " + fileName + "."));
    }

    GLenum format = getFormat(channelsNumber);
    glBindTexture(GL_TEXTURE_2D, textureIdex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);

}



    GLenum LoadTexture::getFormat(const unsigned& channelsNumber)
    {
        if (channelsNumber == 1)
            return GL_RED;
        else if (channelsNumber == 3)
            return GL_RGB;
        else if (channelsNumber == 4)
            return GL_RGBA;
        else throw runtime_error("Can't detected the format of the image!");
    }

*/
