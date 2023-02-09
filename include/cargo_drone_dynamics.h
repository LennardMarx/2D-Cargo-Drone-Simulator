#pragma once
#include <array>
#include "drone_dynamics.h"

class CargoDroneDynamics : public DroneDynamics
{
public:
    CargoDroneDynamics();           // constructor
    ~CargoDroneDynamics() override; // destructor

    void ropeStates(std::array<double, 9> &); // method to calculate rope states

    // overridden euler methods
    void forwardEuler() override;
    void fEuler(std::array<double, 9>, std::array<double, 2> &) override;

    // overridden runge kutta method
    std::array<double, 9> f(std::array<double, 9>, std::array<double, 2> &) override;

private:
    std::array<double, 2> y; // Rope states

    const double hEuler = 0.0005; // euler timestep for cargo drone

    // rope states
    double Frope, FropeStar;
    double Frope_x, Frope_y;

    // rope parameters
    const int Krope = 40000;
    const int Drope = 50;
    const int Lrope_0 = 1.5;

    const int mCargo = 2; // cargo mass
};