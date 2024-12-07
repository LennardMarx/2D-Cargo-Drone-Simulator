#pragma once
#include <array>
#include "drone.h"

class CargoDrone : public Drone
{
public:
    CargoDrone(double, double);
    ~CargoDrone() override;

private:
};

// explanation of emptyness in documentation