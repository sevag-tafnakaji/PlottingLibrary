
#include <Plotter/texture.h>

#include <iostream>

Texture2D::Texture2D()
    : Width(0),
      Height(0),
      Internal_Format(GL_RGB),
      Image_Format(GL_RGB),
      Filter_Min(GL_NEAREST),
      Filter_Max(GL_NEAREST),
      Data()
{
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char *data)
{
    this->Width = width;
    this->Height = height;
    this->Data = data;

    // create texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format,
                 GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const { glBindTexture(GL_TEXTURE_2D, this->ID); }
