#include <Plotter/Plotter.h>

#define SCALE_VALUE 1.05
#define OFFSET_X_VALUE 0.01

float offset_x = 0.0f;
float offset_y = 0.0f;
float scale = 1.0f;

Plotter::Plotter(std::string title, bool fullscreen, int height, int width)
{
    this->SCR_WIDTH = width;
    this->SCR_HEIGHT = height;

    this->init(title, fullscreen);
}

void Plotter::init(std::string title, bool fullscreen)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    if (fullscreen)
        this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, title.c_str(), glfwGetPrimaryMonitor(), NULL);
    else
        this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, title.c_str(), NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: Raise some custom failed init error
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // TODO: Raise some custom failed init error
    }

    Shader PlotterShader = ResourceManager::LoadShader("../resources/shaders/plot.vs", "../resources/shaders/plot.fs", nullptr, "plotter");
}

void Plotter::plot(std::vector<double> x, std::vector<double> y, glm::vec3 colour)
{
    if (glm::all(glm::equal(colour, {0.0f, 0.0f, 0.0f})))
        colour = {glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1)};

    Line line(x, y, colour);

    plotLines.push_back(line);
}

void Plotter::render()
{
    // Extract what the maximum x & y values are
    extractMinMaxValues();

    // use the max x & y values to scale data of each line to [-0.5, 0.5], then store as RenderData struct
    loadDataToBuffers();

    auto start = std::chrono::high_resolution_clock::now();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(this->window))
    {
        // input
        // -----
        processInput(this->window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the current shader
        Shader shader = ResourceManager::GetShader("plotter");

        shader.Use();

        // Update zoom + pan with values from processInput
        glUniform1f(glGetUniformLocation(shader.ID, "scale"), scale);
        glUniform1f(glGetUniformLocation(shader.ID, "offset_x"), offset_x);
        glUniform1f(glGetUniformLocation(shader.ID, "offset_y"), offset_y);

        // plot all of the active buffers
        for (RenderData renderBuffers : activeVAOs)
        {
            shader.SetVector3f("colour", renderBuffers.colour, false);
            glBindVertexArray(renderBuffers.VAO);
            glDrawArrays(renderBuffers.mode, 0, renderBuffers.bufferSize);
        }

        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(this->window);
        glfwPollEvents();

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // std::cout << duration.count() << " ms (" << 1000.0 / duration.count() << " FPS)." << std::endl;

        start = end;
    }
}

void Plotter::extractMinMaxValues()
{
    for (Line l : plotLines)
    {
        std::vector<double> x = l.getX();
        std::vector<double> y = l.getY();
        auto xMinMax = std::minmax_element(x.begin(), x.end());
        auto yMinMax = std::minmax_element(y.begin(), y.end());

        if (*xMinMax.first < xMin)
            xMin = *xMinMax.first;

        if (*xMinMax.second > xMax)
            xMax = *xMinMax.second;

        if (*yMinMax.first < yMin)
            yMin = *yMinMax.first;

        if (*yMinMax.second > yMax)
            yMax = *yMinMax.second;
    }
}

void Plotter::loadDataToBuffers()
{
    for (Line line : plotLines)
    {
        RenderData data = line.loadDataToBuffers(xMin, xMax, yMin, yMax, 
                                                 -1.0, 1.0, -1.0, 1.0,
                                                 GL_LINE_STRIP);

        updateBuffers(data);
    }
}

void Plotter::updateBuffers(RenderData data)
{
    VAOs.push_back(data);
    activeVAOs.push_back(data);
}

Plotter::~Plotter()
{ 
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            offset_x += OFFSET_X_VALUE;
        }

        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            offset_x -= OFFSET_X_VALUE;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            offset_y += OFFSET_X_VALUE;
        }

        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            offset_y -= OFFSET_X_VALUE;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        scale *= SCALE_VALUE;
    }

    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        scale /= SCALE_VALUE;
    }
    
    else if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
    {
        offset_x = 0.0f;
        offset_y = 0.0f;
        scale = 1.0f;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}