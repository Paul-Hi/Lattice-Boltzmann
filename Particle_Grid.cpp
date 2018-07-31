//
// Created by paulh on 26.07.2018.
//

#include "Particle_Grid.h"

Particle_Grid::Particle_Grid(int w, int h, std::array<double, 9> in, int** boundaryList, int numBoundariesInList) {
    _width = w;
    _height = h;
    _init = in;
    _boundaries = boundaryList;
    _numBoundaries = numBoundariesInList;
    allocSpace();
    for(int x = 0; x < _width; x++)
    {
        for(int y = 0; y < _height; y++)
        {
            if(x == 0 && 0 < y  && y < _height - 1)
            {
                _grid[x][y] = Node(_init);
            }
            else
            {
                _grid[x][y] = Node(_init_df);
            }
        }
    }
}

Particle_Grid::~Particle_Grid() {
    freeSpace();
}

void Particle_Grid::collide(double omega) {
    #pragma omp parallel for
    for(int x = 0; x < _width; x++)
    {
        for(int y = 0; y < _height; y++)
        {
            _grid[x][y].collide(omega);
        }
    }
}

void Particle_Grid::stream(int **boundaryCoords, int num) {
    #pragma omp parallel for
    for(int x = 1; x < _width - 1; x++)
    {
        for(int y = 1; y + 1 < _height; y++)
        {
            _grid[x][y].distributions[0] = _old_grid[x + 1][y + 1].distributions[0];
            _grid[x][y].distributions[1] = _old_grid[x][y + 1].distributions[1];
            _grid[x][y].distributions[2] = _old_grid[x - 1][y + 1].distributions[2];
            _grid[x][y].distributions[3] = _old_grid[x + 1][y].distributions[3];
            _grid[x][y].distributions[5] = _old_grid[x - 1][y].distributions[5];
            _grid[x][y].distributions[6] = _old_grid[x + 1][y - 1].distributions[6];
            _grid[x][y].distributions[7] = _old_grid[x][y - 1].distributions[7];
            _grid[x][y].distributions[8] = _old_grid[x - 1][y - 1].distributions[8];
        }
    }
    #pragma omp parallel for
    for(int i = 0; i < num; i++) {
        int *x_y = boundaryCoords[i];
        _grid[ x_y[0]][x_y[1]].distributions = _init_df;
    }
}

void Particle_Grid::step(double omega) {
    for(int i = 0; i < 2; i++)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        in_and_out_flow();
        collide(omega);
        placeBoundaries(_boundaries, _numBoundaries);
        #pragma omp parallel for
        for(int x = 0; x < _width; x++)
        {
            memcpy(_old_grid[x], _grid[x], sizeof(Node) * _height); // NOLINT
        }
        stream(_boundaries, _numBoundaries);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - begin;
        std::cout << "Step in: "<< elapsed.count() << std::endl;
    }
}

void Particle_Grid::draw() {
    for(unsigned long y = 0; y < _height; y++)
    {
        for(unsigned long x = 0; x < _width; x++)
        {
            if(y == 0 || y == _height - 1)
            {
                std::cout << "---";
                continue;
            }
            if(x == _width - 1)
            {
                std::cout << "|~~";
                continue;
            }
            Node n = _grid[x][y];

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

void Particle_Grid::in_and_out_flow() {
    #pragma omp parallel for
    for(unsigned long y = 1; y < _height - 1; y++)
    {
        _grid[0][y] = Node(_init);
        _grid[_width - 2][y] = _grid[_width - 1][y];
    }
}

void Particle_Grid::allocSpace()
{
    _grid = new Node*[_width];
    _old_grid = new Node*[_width];
    for (int i = 0; i < _width; i++)
    {
        _grid[i] = new Node[_height];
    }
    for (int i = 0; i < _width; i++)
    {
        _old_grid[i] = new Node[_height];
    }
}

void Particle_Grid::freeSpace() {
    for (int i = 0; i < _width; i++)
    {
        delete[] (_grid[i]);
    }
    for (int i = 0; i < _width; i++)
    {
        delete[] (_old_grid[i]);
    }
    delete[] (_grid);
    delete[] (_old_grid);
}

void Particle_Grid::placeBoundaries(int **coords, int num) {
    #pragma omp parallel for
    for(int i = 0; i < num; i++)
    {
        int * x_y = coords[i];
        int x_coord = x_y[0];
        int y_coord = x_y[1];
        if(x_coord + 1 < _width && y_coord + 1 < _height) _grid[x_coord][y_coord].distributions[8] = _old_grid[x_coord + 1][y_coord + 1].distributions[0];
        if(y_coord + 1 < _height)                        _grid[x_coord][y_coord].distributions[7] = _old_grid[x_coord][y_coord + 1].distributions[1];
        if(x_coord - 1 > 0 && y_coord + 1 < _height)     _grid[x_coord][y_coord].distributions[6] = _old_grid[x_coord - 1][y_coord + 1].distributions[2];
        if(x_coord + 1 < _width)                         _grid[x_coord][y_coord].distributions[5] = _old_grid[x_coord + 1][y_coord].distributions[3];
        if(x_coord - 1 > 0)                             _grid[x_coord][y_coord].distributions[3] = _old_grid[x_coord - 1][y_coord].distributions[5];
        if(x_coord + 1 < _width && y_coord - 1 > 0)      _grid[x_coord][y_coord].distributions[2] = _old_grid[x_coord + 1][y_coord - 1].distributions[6];
        if(y_coord - 1 > 0)                             _grid[x_coord][y_coord].distributions[1] = _old_grid[x_coord][y_coord - 1].distributions[7];
        if(x_coord - 1 > 0 && y_coord > 0)              _grid[x_coord][y_coord].distributions[0] = _old_grid[x_coord - 1][y_coord - 1].distributions[8];
    }
}