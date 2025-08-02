#include <Plotter/line.h>

Line::Line() {}

Line::Line(std::vector<double> x, std::vector<double> y, glm::vec3 colour)
{
    this->data.setData(x, y);
    this->lineColour = colour;
}

Line::~Line() {}

void Line::setData(std::vector<double> x, std::vector<double> y)
{
    this->data.setData(x, y);
}

RenderData Line::loadDataToBuffers(double xMin, double xMax, double yMin, double yMax, 
                                   double minWidth, double maxWidth, double minHeight, double maxHeight, 
                                   GLenum mode)
{
    Data2D scaledData = Data2D::scaleData(data, xMin, xMax, yMin, yMax, minWidth, maxWidth, minHeight, maxHeight);
    int dataSize = data.size();
    
    float vertices[dataSize * 2];
    
    Data2D::convertToArray(scaledData, vertices);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // magic number are colours. TODO: Add colours later on.
    return RenderData{VAO, dataSize, this->lineColour, -1.0f, mode};
}