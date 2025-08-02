#include <Plotter/Plotter.h>

#define NUM_POINTS 200

int main()
{
    Plotter testPlot{"Test Figure"};

    std::vector<double> t;
    std::vector<double> k;

    double t_min = -10.0;
    double t_max = 10.0;

    double delta_t = (t_max - t_min) / NUM_POINTS;

    for (int i = 0; i <= NUM_POINTS; i++)
    {
        double curr_t = t_min + delta_t * i;
        t.push_back(curr_t);
        k.push_back(2 * curr_t * curr_t + 10);
    }

    testPlot.plot(t, k);

    testPlot.render();

    return 0;
}