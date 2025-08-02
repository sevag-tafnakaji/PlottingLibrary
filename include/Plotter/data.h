#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <string>

// Error class for when the X and Y data vectors are of different sizes
class IncompatibleSize : public std::exception {
private:
    std::string message;
public:

    // Constructor accepting const char*
    IncompatibleSize(const char* msg) :
    message(msg) {}

    // Override what() method, marked
    // noexcept for modern C++
    const char* what() const noexcept {
        return message.c_str();
    }
};

class Data2D
{
public:
    Data2D();
    Data2D(std::vector<double> x, std::vector<double> y);
    ~Data2D();

    void setData(std::vector<double> x, std::vector<double> y);

    std::vector<double> getX() {return X;}

    std::vector<double> getY() {return Y;}

    void setX(std::vector<double> x) {X = x;}

    void setY(std::vector<double> y) {Y = y;}

    int size() {return X.size();}

    static double scalePoint(double raw, double oldMin, double oldMax, double newMin, double newMax);

    static Data2D scaleData(Data2D data, double xMin, double xMax, double yMin, double yMax, 
                    double minWidth, double maxWidth, double minHeight, double maxHeight);
    
    static void convertToArray(Data2D data, float* array);

private:
    std::vector<double> X;
    std::vector<double> Y;

    static bool checkDataSizes(std::vector<double> x, std::vector<double> y);
};

#endif  // DATA_H

