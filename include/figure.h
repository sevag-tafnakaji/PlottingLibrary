#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

class Figure
{
public:
    Figure();
    ~Figure();

    void render();

private:
    // settings
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;

    // pointer to figure window
    GLFWwindow *window;

    void init();
};



#endif  // WINDOW_H