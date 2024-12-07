#include <cmath>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "../include/drone_dynamics.h"
#include <vector>

DroneDynamics::DroneDynamics() // constructor
{
	// initial states
	setUpdatedStates({0, 0, 0, 0, 0, 0, 0, 0, 0});
	setReceivedStates({0, 0, 0, 0, 0, 0, 0, 0, 0});
	setReceivedInputs({3 * 9.81, 0});
}
DroneDynamics::~DroneDynamics() {} // destructor

// get and set functions
bool DroneDynamics::getReset()
{
	return reset;
}
void DroneDynamics::setReset(bool _setup)
{
	reset = _setup;
}

std::array<double, 2> DroneDynamics::getReceivedInputs()
{
	return receivedInputs;
}
void DroneDynamics::setReceivedInputs(std::array<double, 2> _u)
{
	receivedInputs = _u;
}

std::array<double, 9> DroneDynamics::getReceivedStates()
{
	return receivedStates;
}
void DroneDynamics::setReceivedStates(const std::array<double, 9> &_states)
{
	receivedStates = _states;
}

std::array<double, 9> DroneDynamics::getUpdatedStates()
{
	return updatedStates;
}
void DroneDynamics::setUpdatedStates(std::array<double, 9> _xdot)
{
	updatedStates = _xdot;
}

// forward euler method
void DroneDynamics::forwardEuler()
{
	fEuler(receivedStates, receivedInputs);
}
// method that does the integration step
void DroneDynamics::fEuler(std::array<double, 9> _states, std::array<double, 2> &_u)
{
	std::array<double, 9> _output;
	_output.at(0) = _states.at(0) + h * (_states.at(3));
	_output.at(1) = _states.at(1) + h * (_states.at(4));
	_output.at(2) = _states.at(2) + h * (_u.at(1));
	_output.at(3) = _states.at(3) + h * ((-_u.at(0) * sin(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) / m);
	_output.at(4) = _states.at(4) + h * (((_u.at(0) * cos(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) / m) - g);
	_output.at(5) = 0;
	_output.at(6) = 0;
	_output.at(7) = 0;
	_output.at(8) = 0;
	setUpdatedStates(_output);
}
// runge kutta method calculating the intermediate steps
void DroneDynamics::rungeKutta()
{
	std::array<double, 9> _updatedStates;
	K1 = f(receivedStates, receivedInputs);
	K2 = f(addArrays(receivedStates, K1, 2), receivedInputs);
	K3 = f(addArrays(receivedStates, K2, 2), receivedInputs);
	K4 = f(addArrays(receivedStates, K3, 1), receivedInputs);
	for (int i = 0; i < 9; ++i)
	{
		_updatedStates.at(i) = receivedStates.at(i) + (h * ((K1.at(i) + 2 * K2.at(i) + 2 * K3.at(i) + K4.at(i)) / 6));
	}
	setUpdatedStates(_updatedStates);
}
// the method to integrate the runge kutta intermediate steps
std::array<double, 9> DroneDynamics::f(std::array<double, 9> _states, std::array<double, 2> &_u)
{
	std::array<double, 9> _output;
	_output.at(0) = _states.at(3);
	_output.at(1) = _states.at(4);
	_output.at(2) = _u.at(1);
	_output.at(3) = (-_u.at(0) * sin(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) / m;
	_output.at(4) = ((_u.at(0) * cos(_states.at(2)) - C_drag * sqrt(pow(_states.at(3), 2) + pow(_states.at(4), 2)) * _states.at(3)) / m) - g;
	_output.at(5) = 0;
	_output.at(6) = 0;
	_output.at(7) = 0;
	_output.at(8) = 0;
	return _output;
}

// method to add each position of an array with the according position of another
// _over to account for the multiplication with h/2 or h in the runge kutta steps
std::array<double, 9> DroneDynamics::addArrays(std::array<double, 9> _x, std::array<double, 9> _K, double _over)
{
	std::array<double, 9> addedArray;
	for (int i = 0; i < 9; ++i)
	{
		addedArray.at(i) = _x.at(i) + _K.at(i) * h / _over;
	}
	return addedArray;
}
