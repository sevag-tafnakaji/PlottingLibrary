#ifndef LINE_H
#define LINE_H

#include <Plotter/data.h>

class Line
{
public:
    Line();
    Line(std::vector<double> x, std::vector<double> y);
    ~Line();

    void setData(std::vector<double> x, std::vector<double> y);

    void render();

private:
    Data2D data;

    void init();
};

#endif  // LINE_H