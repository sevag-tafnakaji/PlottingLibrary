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

double Data2D::scalePoint(double raw, double oldMin, double oldMax, double newMin, double newMax)
{
    double oldRange = (oldMax - oldMin);
    double newRange = (newMax - newMin);
    double newValue = (((raw - oldMin) * newRange) / oldRange) + newMin;

    return newValue;
}

Data2D Data2D::scaleData(Data2D data, double xMin, double xMax, double yMin, double yMax, 
                         double minWidth, double maxWidth, double minHeight, double maxHeight)
{
    int dataSize = data.size();
    std::vector<double> scaledX;
    std::vector<double> scaledY;

    std::vector<double> x = data.getX();
    std::vector<double> y = data.getY();

    for (int i = 0; i < dataSize; i++)
    {
        double scaledCurrentX = Data2D::scalePoint(x[i], xMin, xMax, minWidth, maxWidth); 
        double scaledCurrentY = Data2D::scalePoint(y[i], yMin, yMax, minHeight, maxHeight);
        scaledX.push_back(scaledCurrentX);
        scaledY.push_back(scaledCurrentY);
    }

    return Data2D(scaledX, scaledY);
}

void Data2D::convertToArray(Data2D data, float* array)
{
    int dataSize = data.size();
    std::vector<double> x = data.getX();
    std::vector<double> y = data.getY();
    int j = 0;
    for (int i = 0; i < (int)(dataSize * 2); i += 2)
    {
        array[i] = (float)x[j];
        array[i + 1] = (float)y[j];
        j++;
    }
}

Data2D::~Data2D() {}
