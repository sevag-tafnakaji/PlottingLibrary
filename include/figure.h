#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <chrono>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

class Figure
{
public:
    Figure(std::string title = "Plotter", bool fullscreen = false, int height = 600, int width = 800);
    ~Figure();

    void render();

private:
    // settings
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;

    // pointer to figure window
    GLFWwindow *window;
    
    void init(std::string title, bool fullscreen);
};



#endif  // WINDOW_H