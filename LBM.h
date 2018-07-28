//
// Created by paulh on 26.07.2018.
//

#ifndef LBC_LBM_H
#define LBC_LBM_H

#include "Particle_Grid.h"


class LBM {

public:
    /* Constructs a Lattice-Boltzmann-Simulation
     * Parameters:
     * width, height : width and height for the resolution of the simulation
     * inflow : inflow values for the distribuions
     * boundaries : boundaries (no slip -> bounce back condition) given with x,y coordinates
     * numBoundaries : number of coordinates given in boundaries
     * omega : omega value for viscosity
     * */
    LBM(int width, int height, std::array<double, 9> inflow, int** boundaries, int numBoundaries, double omega = 1.85);
    /* Start the simulation
     * Parameters:
     * frames : number of steps to simulate
     * */
    bool start(int frames);
    ~LBM();
private:
    // Particle Grid for simulation
    Particle_Grid *pg;
    // width and height of the Particle Grid == resolution of the simulation
    int width, height;
    // omega -> viscosity of the fluid
    double omega;
};


#endif //LBC_LBM_H
