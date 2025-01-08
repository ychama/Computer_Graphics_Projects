#ifndef PARAMETERS_H
#define PARAMETERS_H

struct Parameters
{
    // Default to 1, but if you really need it to be a float, change to float iterationStep = 1.f;
    int iterationStep = 1;

    int scene = 1;
    int iteration = 0;

    bool isDifferent(const Parameters &p) const
    {
        return p.scene != scene || p.iteration != iteration;
    }
};

#endif // PARAMETERS_H