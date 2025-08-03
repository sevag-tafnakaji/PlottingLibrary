#include <Plotter/Plotter.h>

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
    glViewport(FIGURE_MARGIN + TICK_SIZE, FIGURE_MARGIN + TICK_SIZE, SCR_WIDTH - FIGURE_MARGIN * 2 - TICK_SIZE, SCR_HEIGHT - FIGURE_MARGIN * 2 - TICK_SIZE);    
    glDisable(GL_SCISSOR_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    ResourceManager::LoadShader("../resources/shaders/line.vs", "../resources/shaders/line.fs", nullptr, "linePlotter");
    ResourceManager::LoadShader("../resources/shaders/scatter.vs", "../resources/shaders/scatter.fs", nullptr, "scatterPlotter");

    ResourceManager::LoadTexture("../resources/textures/cross.png", true, "cross");
    ResourceManager::LoadTexture("../resources/textures/circle.png", true, "circle");
}

void Plotter::plot(std::vector<double> x, std::vector<double> y, glm::vec3 colour)
{
    if (glm::all(glm::equal(colour, {0.0f, 0.0f, 0.0f})))
        colour = {glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1)};

    Line line(x, y, colour);

    plotLines.push_back(line);
}

void Plotter::scatter(std::vector<double> x, std::vector<double> y, float pointSize, glm::vec3 colour, std::string texture)
{
    if (glm::all(glm::equal(colour, {0.0f, 0.0f, 0.0f})))
        colour = {glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1), glm::linearRand<float>(0, 1)};

    Scatter scatter(x, y, pointSize, colour, texture);

    scatterPlots.push_back(scatter);
}

void Plotter::render()
{
    // Extract what the maximum x & y values are
    extractMinMaxValues();

    // use the max x & y values to scale data of each line to [-0.5, 0.5], then store as RenderData struct
    loadDataToBuffers();

    plotAxes();

    plotTicks();

    auto start = std::chrono::high_resolution_clock::now();

    glm::mat4 transform;

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
        Shader lineShader = ResourceManager::GetShader("linePlotter");

        lineShader.Use();

        // Update zoom + pan with values from processInput
        GLint transformLocation = glGetUniformLocation(lineShader.ID, "transform");
        transform = viewportTransform(FIGURE_MARGIN + TICK_SIZE, 
                                       FIGURE_MARGIN + TICK_SIZE, 
                                       SCR_WIDTH - FIGURE_MARGIN * 2 - TICK_SIZE, 
                                       SCR_HEIGHT - FIGURE_MARGIN * 2 - TICK_SIZE);
        
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        // plot all of the active buffers
        for (RenderData renderBuffers : activeVAOs)
        {
            if (renderBuffers.mode == GL_POINTS)
            {
                Shader scatterShader = ResourceManager::GetShader("scatterPlotter");
                scatterShader.Use();
                if (renderBuffers.texture.Width != 0)
                    renderBuffers.texture.Bind();
                glUniformMatrix4fv(glGetUniformLocation(scatterShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform)); 
                glUniform1i(glGetUniformLocation(scatterShader.ID, "image"), 0); 
                glUniform1f(glGetUniformLocation(scatterShader.ID, "pointSize"), renderBuffers.pointSize); 
                scatterShader.SetVector3f("colour", renderBuffers.colour, false);
            }
            else
                lineShader.Use();
                lineShader.SetVector3f("colour", renderBuffers.colour, false);
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

void Plotter::plotAxes()
{
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    float box[6] = {-1, 1, -1, -1, 1, -1};
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    updateBuffers(RenderData{VAO, 3, glm::vec3(1.0f, 1.0f, 1.0f), -1.0, GL_LINE_STRIP});
}

// TODO: use generalised values instead of magic numbers
void Plotter::plotTicks()
{
    float pixel_x = 2.0 / (SCR_WIDTH - FIGURE_MARGIN * 2 - TICK_SIZE);
    float pixel_y = 2.0 / (SCR_HEIGHT - FIGURE_MARGIN * 2 - TICK_SIZE);

    float tickSpacing = 0.05 * powf(10, -floor(log10(scale)));

    /*
    ---------------------
    ------ x axis -------
    ---------------------
    */
    float left = - 1.0 / scale - offset_x;
    float right = 1.0 / scale - offset_x;

    int leftI = ceil(left / tickSpacing);
    int rightI = floor(right / tickSpacing);

    float remX = leftI * tickSpacing - left;

    float firstTickX = -1.0 + remX * scale;

    int nTicksX = rightI - leftI + 1;

    std::cout << "Num Ticks X: " << nTicksX << std::endl;

    if (nTicksX > 41)
        nTicksX = 41;

    GLuint VBOX, VAOX;
    glGenVertexArrays(1, &VAOX);
    glGenBuffers(1, &VBOX);

    float ticksX[nTicksX * 4];

    int j = 0;
    for(int i = 0; i < nTicksX * 4; i+=4)
    {
        float x = firstTickX + j * tickSpacing * scale;
        // every 10th tick is a major tick - double size
        float tickScale = ((j + leftI) % 10) ? 0.5 : 1;
        ticksX[i] = x;
        ticksX[i + 1] = -1;
        ticksX[i + 2] = x;
        ticksX[i + 3] = -1 - TICK_SIZE * tickScale * pixel_y;
        j++;
    }

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOX);

    glBindBuffer(GL_ARRAY_BUFFER, VBOX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ticksX), ticksX, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // magic number are colours. TODO: Add colours later on.
    updateBuffers(RenderData{VAOX, nTicksX * 4, {1.0f, 1.0f, 1.0f}, -1.0f, GL_LINES});

    /*
    ---------------------
    ------ y axis -------
    ---------------------
    */
    float down = - 1.0 / scale - offset_y;
    float up = 1.0 / scale - offset_y;

    int downI = ceil(down / tickSpacing);
    int upI = floor(up / tickSpacing);

    float remY = downI * tickSpacing - down;

    float firstTickY = -1.0 + remY * scale;

    int nTicksY = upI - downI + 1;

    std::cout << "Num Ticks Y: " << nTicksY << std::endl;

    if (nTicksY > 41)
        nTicksY = 41;

    GLuint VBOY, VAOY;
    glGenVertexArrays(1, &VAOY);
    glGenBuffers(1, &VBOY);

    float ticksY[nTicksY * 4];

    j = 0;
    for(int i = 0; i < nTicksY * 4; i+=4)
    {
        float y = firstTickY + j * tickSpacing * scale;
        // every 10th tick is a major tick - double size
        float tickScale = ((j + downI) % 10) ? 0.5 : 1;
        ticksY[i] = -1;
        ticksY[i + 1] = y;
        ticksY[i + 2] = -1 - TICK_SIZE * tickScale * pixel_x;
        ticksY[i + 3] = y;
        j++;
    }

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOY);

    glBindBuffer(GL_ARRAY_BUFFER, VBOY);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ticksY), ticksY, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    updateBuffers(RenderData{VAOY, nTicksY * 4, {1.0f, 1.0f, 1.0f}, -1.0f, GL_LINES});

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

    for (Scatter s : scatterPlots)
    {
        std::vector<double> x = s.getX();
        std::vector<double> y = s.getY();
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

    for (Scatter scatter : scatterPlots)
    {
        RenderData data = scatter.loadDataToBuffers(xMin, xMax, yMin, yMax, 
                                                    -1.0, 1.0, -1.0, 1.0,
                                                    GL_POINTS);

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

glm::mat4 Plotter::viewportTransform(float x, float y, float width, float height) {
  // Calculate how to translate the x and y coordinates:
  float offset_x = (2.0 * x + (width - SCR_WIDTH)) / SCR_WIDTH;
  float offset_y = (2.0 * y + (height - SCR_HEIGHT)) / SCR_HEIGHT;

  // Calculate how to rescale the x and y coordinates:
  float scale_x = width / SCR_WIDTH;
  float scale_y = height / SCR_HEIGHT;

  return glm::scale(glm::translate(glm::mat4(1), glm::vec3(offset_x, offset_y, 0)), glm::vec3(scale_x, scale_y, 1));
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
    // glViewport(0, 0, width, height);
    glViewport(FIGURE_MARGIN + TICK_SIZE, FIGURE_MARGIN + TICK_SIZE, width - FIGURE_MARGIN * 2 - TICK_SIZE, height - FIGURE_MARGIN * 2 - TICK_SIZE);
}