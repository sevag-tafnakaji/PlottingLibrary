#ifndef WINDOW_H
#define WINDOW_H

#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Plotter/line.h>
#include <Plotter/scatter.h>
#include <Plotter/resource_manager.h>

#define SCALE_VALUE 1.05
#define OFFSET_X_VALUE 0.01

#define FIGURE_MARGIN 20
#define TICK_SIZE 10


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

class Plotter
{
public:
    Plotter(std::string title = "Plotter", bool fullscreen = false, int height = 600, int width = 800);
    ~Plotter();

    void render();

    void plot(std::vector<double> x, std::vector<double> y, glm::vec3 colour = {0.0f, 0.0f, 0.0f});

    void scatter(std::vector<double> x, std::vector<double> y, float pointSize = 1.0f, glm::vec3 colour = {0.0f, 0.0f, 0.0f}, std::string texture = "");

private:
    // settings
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;

    double xMin{std::numeric_limits<double>::max()};
    double yMin{std::numeric_limits<double>::max()};
    double xMax{std::numeric_limits<double>::min()};
    double yMax{std::numeric_limits<double>::min()};

    // pointer to plotter window
    GLFWwindow *window;

    // Lines to plot
    std::vector<Line> plotLines;

    std::vector<Scatter> scatterPlots;

    // All VAOs (data, axes, grids, etc) + data related to drawing lines as desired
    std::vector<RenderData> VAOs;

    // only active VAOs + data related to drawing lines as desired
    std::vector<RenderData> activeVAOs;
    
    void init(std::string title, bool fullscreen);

    void extractMinMaxValues();

    void loadDataToBuffers();

    void updateBuffers(RenderData data);

    glm::mat4 viewportTransform(float x, float y, float width, float height);

    void plotAxes();

    void plotTicks();
};

#endif  // WINDOW_H