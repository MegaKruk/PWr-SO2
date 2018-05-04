cmake_minimum_required(VERSION 3.10)
project(DemonBarber)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-pthread -lncurses")

add_executable(DemonBarber main.cpp Interface.cpp Interface.h FleetStreet.cpp FleetStreet.h)