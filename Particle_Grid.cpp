//
// Created by paulh on 26.07.2018.
//

#include "Particle_Grid.h"

Particle_Grid::Particle_Grid(int w, int h, std::array<double, 9> in, int** boundaryList, int numBoundariesInList) {
    width = w;
    height = h;
    init = in;
    boundaries = boundaryList;
    numBoundaries = numBoundariesInList;
    allocSpace();
    for(unsigned long x = 0; x < width; x++)
    {
        for(unsigned long y = 0; y < height; y++)
        {
            if(x == 0 && 0 < y  && y < height - 1)
            {
                grid[x][y] = Node(init);
            }
            else
            {
                grid[x][y] = Node(init_df);
            }
        }
    }
}

Particle_Grid::~Particle_Grid() {
    freeSpace();
}

void Particle_Grid::collide(double omega) {
    for(unsigned long x = 0; x < width; x++)
    {
        for(unsigned long y = 0; y < height; y++)
        {
            grid[x][y].collide(omega);
        }
    }
}

void Particle_Grid::stream(int **boundaryCoords, int num) {
    for(unsigned long x = 0; x < width; x++)
    {
        for(unsigned long y = 0; y < height; y++)
        {

            if(x == 0 || x == width - 1 || y == 0 || y == height - 1)
            {
                grid[x][y].distributions = init_df;
                continue;
            }
            grid[x][y].distributions[0] = old_grid[x + 1][y + 1].distributions[0];
            grid[x][y].distributions[1] = old_grid[x][y + 1].distributions[1];
            grid[x][y].distributions[2] = old_grid[x - 1][y + 1].distributions[2];
            grid[x][y].distributions[3] = old_grid[x + 1][y].distributions[3];
            grid[x][y].distributions[5] = old_grid[x - 1][y].distributions[5];
            grid[x][y].distributions[6] = old_grid[x + 1][y - 1].distributions[6];
            grid[x][y].distributions[7] = old_grid[x][y - 1].distributions[7];
            grid[x][y].distributions[8] = old_grid[x - 1][y - 1].distributions[8];
        }
    }
    int x_coord, y_coord;
    for(int i = 0; i < num; i++) {
        int *x_y = boundaryCoords[i];
        x_coord = x_y[0];
        y_coord = x_y[1];
        grid[x_coord][y_coord].distributions = init_df;
    }
}

void Particle_Grid::step(double omega) {
    for(int i = 0; i < 1; i++)
    {
        inflow(init);
        outflow();
        collide(omega);
        placeBoundaries(boundaries, numBoundaries);
        stream(boundaries, numBoundaries);
    }
}

void Particle_Grid::draw() {
    for(unsigned long y = 0; y < height; y++)
    {
        for(unsigned long x = 0; x < width; x++)
        {
            if(y == 0 || y == height - 1)
            {
                std::cout << "---";
                continue;
            }
            if(x == width - 1)
            {
                std::cout << "|~~";
                continue;
            }
            Node n = grid[x][y];

            double* x_y = n.getVelocities();

            double v_x = x_y[0];
            double v_y = x_y[1];


            double v = (sqrt(SQUARE(100000 * v_x) + SQUARE(100000 * v_y)));

            auto val = long(v * 100000);
            while(val >= 10)
                val /= 10;


            switch (val)
            {
                case 0:
                    std::cout << "   ";
                    break;
                case 1:
                    std::cout << "O  ";
                    break;
                case 2:
                    std::cout << "O  ";
                    break;
                case 3:
                    std::cout << "0  ";
                    break;
                case 4:
                    std::cout << "0  ";
                    break;
                case 5:
                    std::cout << "+  ";
                    break;
                case 6:
                    std::cout << "+  ";
                    break;
                case 7:
                    std::cout << "#  ";
                    break;
                case 8:
                    std::cout << "#  ";
                    break;
                case 9:
                    std::cout << "=  ";
                    break;
                default:
                    break;

            }

        }
        std::cout << std::endl;
    }
    //getchar();
    //system("clear");
}

void Particle_Grid::inflow(std::array<double, 9> init) {
    for(unsigned long y = 1; y < height - 1; y++)
        grid[0][y] = Node(init);
}

void Particle_Grid::allocSpace()
{
    grid = new Node*[width];
    old_grid = new Node*[width];
    for (int i = 0; i < width; i++)
    {
        grid[i] = new Node[height];
    }
    for (int i = 0; i < width; i++)
    {
        old_grid[i] = new Node[height];
    }
}

void Particle_Grid::freeSpace() {
    for (int i = 0; i < width; i++)
    {
        delete[] (grid[i]);
    }
    for (int i = 0; i < width; i++)
    {
        delete[] (old_grid[i]);
    }
    delete[] (grid);
    delete[] (old_grid);
}

void Particle_Grid::outflow() {
    for(unsigned long y = 1; y < height - 1; y++)
        grid[width - 2][y] = grid[width - 1][y];
}

void Particle_Grid::placeBoundaries(int **coords, int num) {
    int x_coord, y_coord;
    for(int i = 0; i < num; i++)
    {
        int * x_y = coords[i];
        x_coord = x_y[0];
        y_coord = x_y[1];
        if(x_coord + 1 < width && y_coord + 1 < height) grid[x_coord][y_coord].distributions[8] = old_grid[x_coord + 1][y_coord + 1].distributions[0];
        if(y_coord + 1 < height)                        grid[x_coord][y_coord].distributions[7] = old_grid[x_coord][y_coord + 1].distributions[1];
        if(x_coord - 1 > 0 && y_coord + 1 < height)     grid[x_coord][y_coord].distributions[6] = old_grid[x_coord - 1][y_coord + 1].distributions[2];
        if(x_coord + 1 < width)                         grid[x_coord][y_coord].distributions[5] = old_grid[x_coord + 1][y_coord].distributions[3];
        if(x_coord - 1 > 0)                             grid[x_coord][y_coord].distributions[3] = old_grid[x_coord - 1][y_coord].distributions[5];
        if(x_coord + 1 < width && y_coord - 1 > 0)      grid[x_coord][y_coord].distributions[2] = old_grid[x_coord + 1][y_coord - 1].distributions[6];
        if(y_coord - 1 > 0)                             grid[x_coord][y_coord].distributions[1] = old_grid[x_coord][y_coord - 1].distributions[7];
        if(x_coord - 1 > 0 && y_coord > 0)              grid[x_coord][y_coord].distributions[0] = old_grid[x_coord - 1][y_coord - 1].distributions[8];
    }

    for(unsigned long x = 0; x < width; x++)
    {
        for(unsigned long y = 0; y < height; y++)
        {
            old_grid[x][y] = grid[x][y];
        }
    }
}