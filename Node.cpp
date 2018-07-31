//
// Created by paulh on 26.07.2018.
//

#include "Node.h"

Node::Node(std::array<double, NUM_DIRECTIONS> init) {
    distributions = init;
}

Node::Node() {
    distributions = _weights;
}


Node::~Node() = default;

double Node::computeDensity() {
   return std::accumulate(distributions.begin(), distributions.end(), double());
}

double* Node::computeVelocity() {
    _density = computeDensity();

    _v_x_y[0] = (distributions[NORTH_EAST] + distributions[EAST] + distributions[SOUTH_EAST]
            - distributions[SOUTH_WEST] - distributions[WEST] - distributions[NORTH_WEST]) / _density;
    _v_x_y[1] = (distributions[SOUTH_EAST] + distributions[SOUTH] + distributions[SOUTH_WEST]
            - distributions[NORTH_WEST] - distributions[NORTH] - distributions[NORTH_EAST]) / _density;

    return _v_x_y;
}

void Node::equilibrium() {
    //density is computed in there
     double* x_y = computeVelocity();

    double v_x = x_y[0];
    double v_y = x_y[1];
    double lastPart = 1.5f * (SQUARE(v_x) + SQUARE(v_y));


    _distributions_eq[CENTER] = _weights[CENTER]         * _density * (1                                                  - lastPart);
    _distributions_eq[NORTH] = _weights[NORTH]           * _density * (1 + 3 * (-v_y)         + 4.5f * SQUARE(-v_y)       - lastPart);
    _distributions_eq[NORTH_EAST] = _weights[NORTH_EAST] * _density * (1 + 3 * (v_x - v_y)    + 4.5f * SQUARE(v_x - v_y)  - lastPart);
    _distributions_eq[EAST] = _weights[EAST]             * _density * (1 + 3 * (v_x)          + 4.5f * SQUARE(v_x)        - lastPart);
    _distributions_eq[SOUTH_EAST] = _weights[SOUTH_EAST] * _density * (1 + 3 * (v_x + v_y)    + 4.5f * SQUARE(v_x + v_y)  - lastPart);
    _distributions_eq[SOUTH] = _weights[SOUTH]           * _density * (1 + 3 * (v_y)          + 4.5f * SQUARE(v_y)        - lastPart);
    _distributions_eq[SOUTH_WEST] = _weights[SOUTH_WEST] * _density * (1 + 3 * (-v_x + v_y)   + 4.5f * SQUARE(-v_x + v_y) - lastPart);
    _distributions_eq[WEST] = _weights[WEST]             * _density * (1 + 3 * (-v_x)         + 4.5f * SQUARE(-v_x)       - lastPart);
    _distributions_eq[NORTH_WEST] = _weights[NORTH_WEST] * _density * (1 + 3 * (-v_x - v_y)   + 4.5f * SQUARE(-v_x - v_y) - lastPart);
}

void Node::collide(double omega) {
    equilibrium();
    int s = distributions.size();
    for (int i = 0; i < s; i++)
        distributions[i] = distributions[i] * (1 - omega) + _distributions_eq[i] * omega;
}

Node::Node(const Node &other) {
    distributions = other.distributions;
    _distributions_eq = other._distributions_eq;
    _density = other._density;
    _v_x_y[0] = other._v_x_y[0];
    _v_x_y[1] = other._v_x_y[1];
    _weights = other._weights;
}

Node& Node::operator=(const Node &other) {
    if(this != &other)
    {
        distributions = other.distributions;
        _distributions_eq = other._distributions_eq;
        _density = other._density;
        _v_x_y[0] = other._v_x_y[0];
        _v_x_y[1] = other._v_x_y[1];
        _weights = other._weights;
        return *this;
    }
}
