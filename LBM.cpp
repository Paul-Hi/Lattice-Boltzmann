//
// Created by paulh on 26.07.2018.
//

#include "LBM.h"

LBM::LBM(int w, int h, std::array<double, 9> init, int** boundaries, int numboundaries, int simScale, double o) {
    _width = w;
    _height = h;
    _omega = o;
    _simulationScale = simScale;
    screen = new Window(w * _simulationScale, h * _simulationScale);
    _pg = new Particle_Grid(_width, _height, init, boundaries, numboundaries);
}

LBM::~LBM() {
    delete(screen);
    delete(_pg);
}

bool LBM::start(int frames) {
    while(frames > 0 && !screen->quit)
    {
        _pg->step(_omega);
        screen->update();
        screen->draw(_pg->getGrid(), _width, _height, _simulationScale);
        frames--;
    }
    screen->saveBMP("simulation_end.bmp");
    return true;
}
