#include <iostream>
#include <vector>
#include "LBM.h"
#include <SDL.h>
#include "Window.h"

int main(int argc, char* argv[]) {
    std::array<double, 9> init {1./36, 1./9, 1.6/36,
                               1./9,  4./9, 1.6/9,
                               1./36, 1./9, 1.6/36};
    int width = 160, height = 90;

    int** boundaries;
    boundaries = new int*[width * 2 + 360];
    for (int i = 0; i < width * 2 + 360; i++)
    {
        boundaries[i] = new int[2];
    }
    for (int i = 0; i < width; i++)
    {
        boundaries[i][0] = boundaries[i + width][0] = i;
        boundaries[i + width][1] = height - 1;
        boundaries[i][1] = 0;
    }

    //blocks
    int counter = width * 2;
    for(int x = 0; x < 3; x++)
    {
        for(int y = 1; y < 61; y++)
        {
            boundaries[counter][0] = 60 + x;
            boundaries[counter][1] = y;
            counter++;
        }
    }
    for(int x = 0; x < 3; x++)
    {
        for(int y = height - 61; y < height - 1; y++)
        {
            boundaries[counter][0] = 90 + x;
            boundaries[counter][1] = y;
            counter++;
        }
    }
    //blocks end


    LBM lbm(width, height, init, boundaries, width * 2 + 360, 5, 1./0.54);

    lbm.start(10000);

    for (int i = 0; i < width * 2 + 360; i++)
    {
        delete[] (boundaries[i]);
    }
    delete[] (boundaries);

    return 0;
}