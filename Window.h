//
// Created by paulh on 28.07.2018.
//

#ifndef LBC_WINDOW_H
#define LBC_WINDOW_H
#include "SDL.h"
#include <iostream>
#include "Node.h"
#include <cmath>

//! get the minimum of two values
#define MIN(x, y) (x) > (y) ? (y) : (x)

class Window {
public:
    /*!
     * Initializes SDL and a window
     * Parameters:
     * width, height : width and height of the window
     */
    Window(int width, int height);
    Window(const Window& other);
    Window& operator=(const Window& other);
    ~Window();
    /*!
     * updates window and input
     */
    void update();
    /*!
     * redraw buffer and render
     * Parameters:
     * grid : simulation grid
     * width, height : width and height of the grid
     * simulationScale : factor to scale the simulation
     */
    void draw(Node** grid, int width, int height, int simulationScale);
    //! saves current buffer
    void saveBMP(std::string name);
    //! should window close?
    bool quit;
private:
    //! window handle
    SDL_Window* _window;
    //! renderer handle
    SDL_Renderer* _renderer;
    //! event for checking input
    SDL_Event _event;
    //! width and height
    int _width, _height;
    /*!
     * interpolate between three colors
     * Parameters:
     * c1, c2, c3 : Input colors
     * res : Output color
     * t : interpolation factor range 0 to 1
     */
    void interpolate_colors(int* c1, int* c2, int* c3, double* res, double t);
};


#endif //LBC_WINDOW_H
