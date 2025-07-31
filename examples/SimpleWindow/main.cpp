#include <Plotter/Plotter.h>

int main()
{
    Plotter testPlot{"Test Figure"};

    testPlot.render();

    // std::cout << "Test Complete" << std::endl;

    return 0;
}