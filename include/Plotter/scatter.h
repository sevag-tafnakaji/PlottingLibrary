#ifndef SCATTER_H
#define SCATTER_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <Plotter/data.h>
#include <Plotter/utils.h>

class Scatter
{
public:
    Scatter();
    Scatter(std::vector<double> x, std::vector<double> y, float size = 1.0f, glm::vec3 colour = {0.0f, 0.0f, 0.0f});
    ~Scatter();

    void setData(std::vector<double> x, std::vector<double> y);

    Data2D getData() {return data;}

    std::vector<double> getX() {return data.getX();}

    std::vector<double> getY() {return data.getY();}

    void render();

    RenderData loadDataToBuffers(double xMin, double xMax, double yMin, double yMax, 
                                 double minWidth, double maxWidth, double minHeight, double maxHeight, 
                                 GLenum mode);

private:
    Data2D data;

    float pointSize;
    glm::vec3 pointColour;
};

#endif  // SCATTER_H