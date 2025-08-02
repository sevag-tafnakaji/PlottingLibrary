
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct RenderData
{
    unsigned int VAO;
    int bufferSize;
    glm::vec3 colour;
    GLenum mode;
};