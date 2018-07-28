//
// Created by paulh on 26.07.2018.
//

#ifndef LBC_NODE_H
#define LBC_NODE_H

#include <array>
#include <numeric>
#include <tuple>
#include <iostream>


#define SQUARE(x) ((x) * (x))

class Node {
public:
    // directions for the distributions
    enum Directions
    {
        NORTH_WEST, NORTH, NORTH_EAST,
        WEST, CENTER, EAST,
        SOUTH_WEST, SOUTH, SOUTH_EAST,
        NUM_DIRECTIONS
    };
    /* Constructs a Node Object
     * Parameters:
     * init : initial distribution values
     * */
    Node(std::array<double, NUM_DIRECTIONS> init);
    Node();
    Node(const Node& other);
    Node& operator=(const Node& other);
    ~Node();
    /* re - distributes the distribution values inside the node
     * Parameters:
     * omega : viscosity parameter
     * */
    void collide(double omega = 1.85f);
    /* Computes and returns x and y velocities inside node
     * Returns:
     * double x_y_velocity[2] : the x and y velocities inside node
     * */
    inline double* getVelocities() {return computeVelocity();};
    //propability values for each neighbouring direction
    std::array<double, NUM_DIRECTIONS> distributions;
    //propability values for each neighbouring direction equally distributed
    std::array<double, NUM_DIRECTIONS> distributions_eq;
private:
    /* Computes and returns x and y velocities inside node
     * Returns:
     * double x_y_velocity[2] : the x and y velocities inside node
     * */
    double* computeVelocity();
    // the x and y velocities inside node
    double v_x_y[2];
    // the density inside node
    double density;
    /* Computes and density inside node
     * Returns:
     * double : the density inside node
     * */
    double computeDensity();
    // Calculates the equally distributed probability values
    void equilibrium();
    // weights for the probability values
    std::array<double, NUM_DIRECTIONS> weights {1./36, 1./9, 1./36,
                                                1./9,  4./9, 1./9,
                                                1./36, 1./9, 1./36};
};



#endif //LBC_NODE_H
