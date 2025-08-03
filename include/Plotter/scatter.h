#ifndef SCATTER_H
#define SCATTER_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <Plotter/data.h>
#include <Plotter/utils.h>
#include <Plotter/resource_manager.h>

// Error class for when the X and Y data vectors are of different sizes
class ConflictingSettings : public std::exception {
private:
    std::string message;
public:

    // Constructor accepting const char*
    ConflictingSettings(const char* msg) :
    message(msg) {}

    // Override what() method, marked
    // noexcept for modern C++
    const char* what() const noexcept {
        return message.c_str();
    }
};

class Scatter
{
public:
    Scatter();
    Scatter(std::vector<double> x, std::vector<double> y, float size = 1.0f, glm::vec3 colour = {0.0f, 0.0f, 0.0f}, std::string texture = "");
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

    std::string pointTexture;
};

#endif  // SCATTER_H