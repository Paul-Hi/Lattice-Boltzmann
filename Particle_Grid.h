//
// Created by paulh on 26.07.2018.
//

#ifndef LBC_PARTICLE_GRID_H
#define LBC_PARTICLE_GRID_H

#include <vector>
#include <iostream>
#include "Node.h"
#include <cmath>


class Particle_Grid {
public:
    /*! Constructs a grid full of nodes for the Lattice-Boltzmann-Simulation
     * Parameters:
     * width, height : width and height for the resolution of the simulation
     * inflow : inflow values for the distribuions
     * boundaries : boundaries (no slip -> bounce back condition) given with x,y coordinates
     * numBoundaries : number of coordinates given in boundaries
     */
    Particle_Grid(int width, int height, std::array<double, 9> inflow, int** boundaries, int numBoundaries);
    ~Particle_Grid();
    /*! Re - distributes the distribution values inside all grid nodes
     * Parameters:
     * omega : viscosity parameter
     */
    void collide(double omega);
    /*! Propagates the distribution values across neighbouring grid nodes
     */
    void stream(int **boundaryCoords, int num);
    /*! Does one simulation step
     * Parameters:
     * omega : viscosity parameter
     */
    void step(double omega = 1.85f);
    /*! Draws the grid in ascii ;)
     */
    void draw();
    /*! Copys inflow values to the starting column
     * Parameters:
     * init :inflow distribution values
     */
    void inflow(std::array<double, 9> init);
    /*! Places no slip boundaries
     * Parameters:
     * coords : list of boundaries given in x,y coordinates
     * num : number of coordinate entries
     */
    void placeBoundaries(int** coords, int num);
    //! Copys outflow values
    void outflow();
    //! Allocates space for the grids
    void allocSpace();
    //! Deletes space for the grids
    void freeSpace();
    //! returns the currently calculated grid
    Node **getGrid() {return grid;};

private:
    //! main grid
    Node** grid;
    //! second grid for calculations
    Node** old_grid;
    //! default init values for distribution values
    std::array<double, 9> init_df {1./36, 1./9, 1./36,
                                   1./9,  4./9, 1./9,
                                   1./36, 1./9, 1./36};
    //! inflow values
    std::array<double, 9> init;
    //! width and height -> resolution // numboundaries -> number of boundarie coordinates
    int width, height, numBoundaries;
    //! boundary list
    int** boundaries;
};


#endif //LBC_PARTICLE_GRID_H
