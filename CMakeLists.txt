cmake_minimum_required(VERSION 3.10)
project(LBC__)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

find_package(OpenMP REQUIRED)

add_executable(LBC__ main.cpp LBM.cpp LBM.h Node.cpp Node.h Particle_Grid.cpp Particle_Grid.h Window.cpp Window.h)

target_compile_options(LBC__ PRIVATE ${OpenMP_CXX_FLAGS})

include_directories(
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        )
target_link_libraries(LBC__
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${OpenMP_CXX_FLAGS}
        )