#include <iostream>
#include <vector>
#include "LBM.h"
int main() {
    std::array<double, 9> init {1./36, 1./9, 1.6/36,
                               1./9,  4./9, 1.6/9,
                               1./36, 1./9, 1.6/36};
    int width = 38, height = 28;

    int** boundaries = new int*[width];
    for (int i = 0; i < width * 2 + 20; i++)
    {
        boundaries[i] = new int[2];
    }
    for (int i = 0; i < width; i++)
    {
        int a [2] = {i, 0};
        int b [2] = {i, height - 1};
        boundaries[i] = a;
        boundaries[i + width] = b;
    }
    int row = 10;
    for (int i = 0; i < 20; i+=4)
    {
        int a [2] = {11, row};
        int b [2] = {12, row};
        int c [2] = {13, row};
        int d [2] = {14, row};
        boundaries[width * 2 + i] = a;
        boundaries[width * 2 + i + 1] = b;
        boundaries[width * 2 + i + 2] = c;
        boundaries[width * 2 + i + 3] = d;
        row++;
    }

    LBM lbm(width, height, init, boundaries, width * 2 + 20, 1.85);

    lbm.start(800);


    getchar();
    for (int i = 0; i < width * 2 + 20; i++)
    {
        delete[] boundaries[i];
    }
    delete[] boundaries;

    return 0;
}