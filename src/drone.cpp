#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "../include/drone.h"

using namespace std::chrono_literals;

Drone::Drone(double _x, double _y) // constructor
{
    setStates({_x, _y, 0, 0, 0, 0, 0, 0, 0});
}
Drone::~Drone() {} // destructor

std::array<double, 9> Drone::getStates() // get function for the drone states
{
    return droneStates;
}
void Drone::setStates(std::array<double, 9> _x) // set function to the drone states
{
    droneStates = _x;
}

bool Drone::getReset() // get and set functions for the reset
{
    return reset;
}
void Drone::setReset(bool _reset)
{
    reset = _reset;
}