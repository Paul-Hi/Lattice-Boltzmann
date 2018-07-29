//
// Created by paulh on 26.07.2018.
//

#include "LBM.h"

LBM::LBM(int w, int h, std::array<double, 9> init, int** boundaries, int numboundaries, int simScale, double o) {
    width = w;
    height = h;
    omega = o;
    simulationScale = simScale;
    screen = new Window(w * simulationScale, h * simulationScale);
    pg = new Particle_Grid(width, height, init, boundaries, numboundaries);
}

LBM::~LBM() {
    delete(screen);
    delete(pg);
}

bool LBM::start(int frames) {
    while(frames > 0 && !screen->quit)
    {
        pg->step(omega);
        screen->update();
        screen->draw(pg->getGrid(), width, height, simulationScale);
        frames--;
    }
    screen->saveBMP("simulation_end.bmp");
    return true;
}
