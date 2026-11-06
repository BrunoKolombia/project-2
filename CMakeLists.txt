cmake_minimum_required(VERSION 3.20)
project(project-2)

set(CMAKE_CXX_STANDARD 20)

add_executable(CineMatch
        main.cpp
        movies.cpp
        recommendations.cpp
        interface.h
)
