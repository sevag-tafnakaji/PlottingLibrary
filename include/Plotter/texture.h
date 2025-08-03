#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D can also store and configure a texture in OpenGL.
// Also hosts utility functions.
class Texture2D
{
public:
    // holds the id of texture object. Used for all texture operations to reference to this specific
    // texture
    unsigned int ID;
    // image dimensions
    unsigned int Width, Height; // in pixels
    // texture format
    unsigned int Internal_Format;
    unsigned int Image_Format;
    // configuration
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    // Image
    unsigned char *Data;

    // constructor
    Texture2D();

    // generate texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char *data);

    // bind the texture as current active GL_TEXTURE_2D
    void Bind() const;
};

#endif