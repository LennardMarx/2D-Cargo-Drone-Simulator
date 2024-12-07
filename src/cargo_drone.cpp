#include "../include/cargo_drone.h"
#include "../include/drone.h"

// cargo drone, that inherits from normal drone
CargoDrone::CargoDrone(double _x, double _y) : Drone(_x, _y)
{
    setStates({_x, _y, 0, 0, 0, _x, _y - 1, 0, 0});
}
CargoDrone::~CargoDrone()
{
}

// explanation of emptyness in documentation