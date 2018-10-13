cmake_minimum_required(VERSION 3.12)
project(ISM_Lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(ISM_Lab4 main.cpp
                        PRNG.h
                        MultiplicativePRNG.h
                        MultiplicativePRNG.cpp)