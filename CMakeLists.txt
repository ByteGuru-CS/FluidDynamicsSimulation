cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(FluidDynamicsSimulation)

add_subdirectory(metal-lib)  # Metal Library

add_subdirectory(src)  #target
