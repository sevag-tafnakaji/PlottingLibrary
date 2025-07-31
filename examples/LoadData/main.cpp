#include <Plotter/data.h>

#include <iostream>

int main()
{
    Data2D data;

    std::vector<double> toyData{1, 2, 3};

    data.setData(toyData, toyData);
    std::cout << "Successfully set new data" << std::endl;

    try
    {
        data.setData({1}, toyData);
    }
    catch(const IncompatibleSize& e)
    {
        std::cout << "Successfully caught incompatible size error" << std::endl;
    }
    
    std::vector<double> x = data.getX();
    std::vector<double> y = data.getY(); 

    bool xMatching = true;
    bool yMatching = true;

    for (int i = 0; i < toyData.size(); i++)
    {
        if (x[i] != toyData[i])
            xMatching = false;

        if (y[i] != toyData[i])
            yMatching = false;
    }

    std::cout << "Stored data matches expected values" << std::endl;

    return 0;
}