#include <Plotter/data.h>

Data2D::Data2D(){}

Data2D::Data2D(std::vector<double> x, std::vector<double> y)
{
    setData(x, y);    
}

void Data2D::setData(std::vector<double> x, std::vector<double> y)
{
    if (Data2D::checkDataSizes(x, y))
    {
        setX(x);
        setY(y);
    }else
    {
        throw IncompatibleSize("X and Y are of different sizes");
    }
}

bool Data2D::checkDataSizes(std::vector<double> x, std::vector<double> y)
{
    return (x.size() == y.size());
}

Data2D::~Data2D() {}
