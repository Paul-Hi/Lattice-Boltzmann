//
// Created by paulh on 26.07.2018.
//

#include "LBM.h"

LBM::LBM(int w, int h, std::array<double, 9> init, int** boundaries, int numboundaries, double o) {
    width = w;
    height = h;
    omega = o;

    pg = new Particle_Grid(width, height, init, boundaries, numboundaries);
}

LBM::~LBM() {
    delete(pg);
}

bool LBM::start(int frames) {
    while(frames > 0)
    {
        pg->step(omega);
        frames--;
    }
    return true;
}
