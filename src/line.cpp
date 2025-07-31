#include <Plotter/line.h>

Line::Line() { this->init();}

Line::Line(std::vector<double> x, std::vector<double> y)
{
    this->data.setData(x, y);
    this->init();
}

Line::~Line() {}

void Line::setData(std::vector<double> x, std::vector<double> y)
{
    this->data.setData(x, y);
}