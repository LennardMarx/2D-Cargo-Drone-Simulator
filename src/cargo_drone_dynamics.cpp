#include <array>
#include "../include/cargo_drone_dynamics.h"
#include "../include/drone_dynamics.h"
#include <cmath>
#include <vector>

// cargo drone dynamics, that inherits from normal drone dynamics
CargoDroneDynamics::CargoDroneDynamics() : DroneDynamics()
{

    setUpdatedStates({0, 0, 0, 0, 0, 0, 0 - 1, 0, 0});
    setReceivedStates({0, 0, 0, 0, 0, 0, 0 - 1, 0, 0});
    setReceivedInputs({5 * 9.81, 0});

    y.at(0) = 1;
    y.at(1) = 0;
}
CargoDroneDynamics::~CargoDroneDynamics() {} // destructor

// overridden Euler method to involve cargo
void CargoDroneDynamics::fEuler(std::array<double, 9> _states, std::array<double, 2> &_u)
{
    ropeStates(_states);
    std::array<double, 9> _output;
    _output.at(0) = _states.at(0) + hEuler * (_states.at(3));
    _output.at(1) = _states.at(1) + hEuler * (_states.at(4));
    _output.at(2) = _states.at(2) + hEuler * (_u.at(1));
    _output.at(3) = _states.at(3) + hEuler * (((-_u.at(0) * sin(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) - Frope_x) / m);
    _output.at(4) = _states.at(4) + hEuler * ((((_u.at(0) * cos(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) - Frope_y) / m) - g);
    _output.at(5) = _states.at(5) + hEuler * (_states.at(7));
    _output.at(6) = _states.at(6) + hEuler * (_states.at(8));
    _output.at(7) = _states.at(7) + hEuler * (((-C_drag * (sqrt(pow(_states.at(7), 2) + pow(_states.at(8), 2)) * _states.at(7))) + Frope_x) / mCargo);
    _output.at(8) = _states.at(8) + hEuler * ((((-C_drag * (sqrt(pow(_states.at(7), 2) + pow(_states.at(8), 2)) * _states.at(8))) + Frope_y) / mCargo) - g);
    setUpdatedStates(_output);
}
// overridden euler method, that does 20 integration steps per ros-time step (10ms)
void CargoDroneDynamics::forwardEuler()
{
    fEuler(receivedStates, receivedInputs);
    for (int i = 0; i < 19; ++i)
    {
        fEuler(updatedStates, receivedInputs);
    }
}

// method to calculate the rope states
void CargoDroneDynamics::ropeStates(std::array<double, 9> &_states)
{
    y.at(0) = sqrt(pow((_states.at(0) - _states.at(5)), 2) + pow((_states.at(1) - _states.at(6)), 2));
    y.at(1) = ((_states.at(0) - _states.at(5)) * (_states.at(3) - _states.at(7)) + (_states.at(1) - _states.at(6)) * (_states.at(4) - _states.at(8))) / y.at(0);
    FropeStar = Krope * (y.at(0) - Lrope_0) + Drope * y.at(1);
    if (FropeStar > 0)
    {
        Frope = FropeStar;
    }
    else
    {
        Frope = 0;
    }
    Frope_x = Frope * (_states.at(0) - _states.at(5)) / y.at(0);
    Frope_y = Frope * (_states.at(1) - _states.at(6)) / y.at(0);
}

// overridden method that does the runge kutta intermediate integration steps
std::array<double, 9> CargoDroneDynamics::f(std::array<double, 9> _states, std::array<double, 2> &_u)
{
    ropeStates(_states);
    std::array<double, 9> _output;
    _output.at(0) = _states.at(3);
    _output.at(1) = _states.at(4);
    _output.at(2) = _u.at(1);
    _output.at(3) = ((-_u.at(0) * sin(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) - Frope_x) / m;
    _output.at(4) = (((_u.at(0) * cos(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) - Frope_y) / m) - g;
    _output.at(5) = _states.at(7);
    _output.at(6) = _states.at(8);
    _output.at(7) = ((-C_drag * (sqrt(pow(_states.at(7), 2) + pow(_states.at(8), 2)) * _states.at(7))) + Frope_x) / mCargo;
    _output.at(8) = (((-C_drag * (sqrt(pow(_states.at(7), 2) + pow(_states.at(8), 2)) * _states.at(8))) + Frope_y) / mCargo) - g;
    return _output;
}

// rungeKutta() method itself is completlely inerited