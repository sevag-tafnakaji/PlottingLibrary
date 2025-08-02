#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <Plotter/data.h>
#include <Plotter/utils.h>

class Line
{
public:
    Line();
    Line(std::vector<double> x, std::vector<double> y);
    ~Line();

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

};

#endif  // LINE_H