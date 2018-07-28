cmake_minimum_required(VERSION 3.10)
project(LBC__)

set(CMAKE_CXX_STANDARD 11)

add_executable(LBC__ main.cpp LBM.cpp LBM.h Node.cpp Node.h Particle_Grid.cpp Particle_Grid.h)