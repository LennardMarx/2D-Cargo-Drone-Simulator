#include <array>
#include "../include/cargo_drone_dynamics.h"
#include "../include/drone_dynamics.h"
#include "../include/controller.h"
#include <cmath>

DroneController::DroneController() // constructor
{
    setReceivedUpdatedStates({0, 0, 0, 0, 0, 0, 0 - 1, 0, 0});

    receivedVel_ref.at(0) = 0;
    receivedVel_ref.at(1) = 0;
}
DroneController::~DroneController() {} // destructor

// get and set methods
std::array<double, 2> DroneController::getReceivedVelref()
{
    return receivedVel_ref;
}
void DroneController::setReceivedVelref(std::array<double, 2> _v_ref)
{
    receivedVel_ref = _v_ref;
}
std::array<double, 9> DroneController::getReceivedUpdatedStates()
{
    return updatedStates;
}
void DroneController::setReceivedUpdatedStates(std::array<double, 9> _states)
{
    updatedStates = _states;
}
std::array<double, 2> DroneController::getInput()
{
    return input;
}
void DroneController::setInput(std::array<double, 2> _input)
{
    input = _input;
}
void DroneController::setDroneType(bool _type)
{
    droneType = _type;
}

// methods for the controller calculations
double DroneController::calculate_d_osc(int D_osc, std::array<double, 9> _states)
{
    return D_osc * (_states.at(5) - _states.at(0));
}

double DroneController::calculate_F_refx(int md, int mc, double tx, std::array<double, 2> _v_ref, std::array<double, 9> _states, double _d_osc)
{
    return ((md + mc) / tx) * (_v_ref.at(0) - _states.at(3)) + _d_osc;
}

double DroneController::calculate_F_refy(int md, int mc, double ty, std::array<double, 2> _v_ref, std::array<double, 9> _states)
{
    return ((md + mc) / ty) * (_v_ref.at(1) - _states.at(4)) + (md + mc) * g;
}

double DroneController::calculate_theta_ref(double _F_refx, double _F_refy)
{
    return atan2(-_F_refx, _F_refy);
}

double DroneController::calculate_thrust(double _F_refy, std::array<double, 9> _states)
{
    return _F_refy / cos(_states.at(2));
}

double DroneController::calculate_omega(double t_theta, double _theta_ref, std::array<double, 9> _states)
{
    return (_theta_ref - _states.at(2)) / t_theta;
}

// calculate the Inputs for the drone dynamics
void DroneController::calculateControllerOutput() // std::array<double, 9> _states, std::array<double, 2> _v_ref
{
    std::array<double, 2> _Inputs;
    if (droneType == 0)
    {
        d_osc = 0;
        F_refx = calculate_F_refx(md, 0, tx, receivedVel_ref, updatedStates, d_osc);
        F_refy = calculate_F_refy(md, 0, ty, receivedVel_ref, updatedStates);
    }
    else if (droneType == 1)
    {
        d_osc = calculate_d_osc(D_osc, updatedStates);
        F_refx = calculate_F_refx(md, mc, tx, receivedVel_ref, updatedStates, d_osc);
        F_refy = calculate_F_refy(md, mc, ty, receivedVel_ref, updatedStates);
    }
    theta_ref = calculate_theta_ref(F_refx, F_refy);
    _Inputs.at(0) = calculate_thrust(F_refy, updatedStates);
    _Inputs.at(1) = calculate_omega(t_theta, theta_ref, updatedStates);
    setInput(_Inputs);
}
