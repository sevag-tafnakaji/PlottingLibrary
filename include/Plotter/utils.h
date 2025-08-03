#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <Plotter/texture.h>

struct RenderData
{
    unsigned int VAO;
    int bufferSize;
    glm::vec3 colour;
    float pointSize;
    GLenum mode;
    Texture2D texture;
};

struct Character
{
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::vec2 Size;    // Size of glyph
    glm::vec2 Bearing; // Offset from baseline to left/top of glyph
    long int Advance;    // Offset to advance to next glyph (learn opengl says unsigned int, but that throws warnings)
};

#endif  // UTILS_H