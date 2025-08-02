#include <Plotter/Plotter.h>
#include <cmath>

#define NUM_POINTS 200

int main()
{
    Plotter testPlot{"Test Figure"};

    std::vector<double> t;
    std::vector<double> k1;
    std::vector<double> k2;
    std::vector<double> k3;

    double t_min = -10.0;
    double t_max = 10.0;

    double delta_t = (t_max - t_min) / NUM_POINTS;

    for (int i = 0; i <= NUM_POINTS; i++)
    {
        double curr_t = t_min + delta_t * i;
        double y1 = 2 * curr_t * curr_t + 10;
        double y2 = curr_t * curr_t - curr_t;
        double y3 = NUM_POINTS * sin(10 * curr_t) / (1 + curr_t * curr_t);
        t.push_back(curr_t);
        k1.push_back(y1);
        k2.push_back(y2);
        k3.push_back(y3);
    }

    testPlot.scatter(t, k1);
    testPlot.scatter(t, k2);
    testPlot.scatter(t, k3);

    testPlot.render();

    return 0;
}