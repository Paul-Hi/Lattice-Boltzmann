//
// Created by paulh on 28.07.2018.
//

#include <vector>
#include "Window.h"

Window::Window(int w, int h) {
    _width = w;
    _height = h;
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        _window = SDL_CreateWindow("Lattice-Boltzmann", 100, 100, _width, _height, SDL_WINDOW_SHOWN);
        if (_window == nullptr){
            std::cout << "SDL Window could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
        else
        {
            _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
            if (_renderer == nullptr){
                std::cout << "SDL Window could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(_window);
                SDL_Quit();
            }
            else
                quit = false;
        }
    }
}

Window::Window(const Window &other) {
    _window = other._window;
    _renderer = other._renderer;
}

Window& Window::operator=(const Window &other) {
    if(this != &other)
    {
        _window = other._window;
        _renderer = other._renderer;
    }
    return *this;
}

Window::~Window() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    SDL_Quit();
}

void Window::update() {
    while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
        }
    }
}

void Window::draw(Node **grid, int width, int height, int simulationScale) {
    auto begin = std::chrono::high_resolution_clock::now();
    int c1[3] ={25, 0, 0};
    int c2[3] ={255, 20, 20};
    int c3[3] ={255, 100, 50};
    //SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    //SDL_RenderClear(_renderer);
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    std::vector< unsigned char > pixels( width * height * 4, 0 );
    #pragma omp parallel for
    for(unsigned long x = 0; x < width; x++)
    {
        for(unsigned long y = 0; y < height; y++)
        {
            double* x_y = grid[x][y].getVelocities();

            double v_x = x_y[0];
            double v_y = x_y[1];

            double v = (sqrt(SQUARE(v_x) + SQUARE(v_y)));
            //map from 0 - 0,1 to 0 - 1
            double t = (v * 10);
            //interpolate between colors
            double color[3];
            interpolate_colors(c1, c2, c3, color, t);
            unsigned int offset = 4 * x + y * width * 4;
            pixels[offset] = static_cast<unsigned char>(color[2]);
            pixels[offset + 1] = static_cast<unsigned char>(color[1]);
            pixels[offset + 2] = static_cast<unsigned char>(color[0]);
            pixels[offset + 3] = SDL_ALPHA_OPAQUE;
        }
    }
    SDL_UpdateTexture(_texture, nullptr, &pixels[0], width * 4);
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(_texture);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - begin;
    std::cout << "Draw in: " << elapsed.count() << std::endl;

}

void Window::interpolate_colors(int *c1, int *c2, int *c3, double *res, double t) {
    double r = 0, g = 0, b = 0;
    if (t <= 0.5)
    {
        r = (c1[0] * (0.5 - t) * 2.0 + c2[0] * t * 2.0);
        g = (c1[1] * (0.5 - t) * 2.0 + c2[1] * t * 2.0);
        b = (c1[2] * (0.5 - t) * 2.0 + c2[2] * t * 2.0);
    }
    else
    {
        r = (c2[0] * (1.0 - t) * 2.0 + c3[0] * (t - 0.5) * 2.0);
        g = (c2[1] * (1.0 - t) * 2.0 + c3[1] * (t - 0.5) * 2.0);
        b = (c2[2] * (1.0 - t) * 2.0 + c3[2] * (t - 0.5) * 2.0);
    }
    res[0] = MIN(r, 255);
    res[1] = MIN(g, 255);
    res[2] = MIN(b, 255);
}

void Window::saveBMP(std::string name) {
    const Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, _width, _height, 32, format);
    SDL_RenderReadPixels(_renderer, NULL, format, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, name.c_str());
    SDL_FreeSurface(surface);
}

