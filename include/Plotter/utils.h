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

#endif  // UTILS_H