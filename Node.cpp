//
// Created by paulh on 26.07.2018.
//

#include "Node.h"

Node::Node(std::array<double, NUM_DIRECTIONS> init) {
    distributions = init;
}

Node::Node() {
    distributions = weights;
}


Node::~Node() = default;

double Node::computeDensity() {
   return std::accumulate(distributions.begin(), distributions.end(), double());
}

double* Node::computeVelocity() {
    density = computeDensity();

    double v_x = (distributions[NORTH_EAST] + distributions[EAST] + distributions[SOUTH_EAST]
            - distributions[SOUTH_WEST] - distributions[WEST] - distributions[NORTH_WEST]) * 1. / density;
    double v_y = (distributions[SOUTH_EAST] + distributions[SOUTH] + distributions[SOUTH_WEST]
            - distributions[NORTH_WEST] - distributions[NORTH] - distributions[NORTH_EAST]) * 1. / density;

    v_x_y[0] = v_x;
    v_x_y[1] = v_y;

    return v_x_y;
}

void Node::equilibrium() {
    //density is computed in there
    double* x_y = computeVelocity();

    double v_x = x_y[0];
    double v_y = x_y[1];

    distributions_eq[CENTER] = weights[CENTER]         * density * (1                                                  - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[NORTH] = weights[NORTH]           * density * (1 + 3 * (-v_y)         + 4.5f * SQUARE(-v_y)       - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[NORTH_EAST] = weights[NORTH_EAST] * density * (1 + 3 * (v_x - v_y)    + 4.5f * SQUARE(v_x - v_y)  - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[EAST] = weights[EAST]             * density * (1 + 3 * (v_x)          + 4.5f * SQUARE(v_x)        - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[SOUTH_EAST] = weights[SOUTH_EAST] * density * (1 + 3 * (v_x + v_y)    + 4.5f * SQUARE(v_x + v_y)  - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[SOUTH] = weights[SOUTH]           * density * (1 + 3 * (v_y)          + 4.5f * SQUARE(v_y)        - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[SOUTH_WEST] = weights[SOUTH_WEST] * density * (1 + 3 * (-v_x + v_y)   + 4.5f * SQUARE(-v_x + v_y) - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[WEST] = weights[WEST]             * density * (1 + 3 * (-v_x)         + 4.5f * SQUARE(-v_x)       - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));
    distributions_eq[NORTH_WEST] = weights[NORTH_WEST] * density * (1 + 3 * (-v_x - v_y)   + 4.5f * SQUARE(-v_x - v_y) - 1.5f * (SQUARE(v_x) + SQUARE(v_y)));

}

void Node::collide(double omega) {
    equilibrium();
    for (unsigned long i = 0; i < distributions.size(); i++)
        distributions[i] = distributions[i] * (1 - omega) + distributions_eq[i] * omega;
}

Node::Node(const Node &other) {
    distributions = other.distributions;
    distributions_eq = other.distributions_eq;
    density = other.density;
    v_x_y[0] = other.v_x_y[0];
    v_x_y[1] = other.v_x_y[1];
    weights = other.weights;
}

Node& Node::operator=(const Node &other) {
    if(this != &other)
    {
        distributions = other.distributions;
        distributions_eq = other.distributions_eq;
        density = other.density;
        v_x_y[0] = other.v_x_y[0];
        v_x_y[1] = other.v_x_y[1];
        weights = other.weights;
        return *this;
    }
}
