# Lattice-Boltzmann
LBM in C++

Lattice-Boltzmann-Solver in C++ without GPU Acceleration, but with multithreading on CPU.
Graphical output width (https://www.libsdl.org/index.php "SDL2").

Dependencies:

SDL2
Compiler supporting OpenMP

Windows:

Change the SDL2PATH to your path in FindSDL2.cmake

Linux:

Change the includes "SDL.h" to "SDL2/SDL.h"
You may need to change SDL2_LIBRARY, SDL2_IMAGE_LIBRARY and SDL2_MIXER_LIBRARY 
                    to SDL2_LIBRARIES, SDL2_IMAGE_LIBRARIES and SDL2_MIXER_LIBRARIES in CMakeLists.txt
