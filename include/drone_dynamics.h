#pragma once
#include <array>
#include <vector>

class DroneDynamics
{
public:
	DroneDynamics();		  // contructor
	virtual ~DroneDynamics(); // destructor

	// get and set methods
	bool getReset();
	void setReset(bool);

	std::array<double, 2> getReceivedInputs();
	void setReceivedInputs(std::array<double, 2>);

	std::array<double, 9> getReceivedStates();
	void setReceivedStates(const std::array<double, 9> &);

	std::array<double, 9> getUpdatedStates();
	void setUpdatedStates(std::array<double, 9>);

	// virtual euler methods to be overridden
	virtual void forwardEuler();
	virtual void fEuler(std::array<double, 9>, std::array<double, 2> &);

	// virtual runge kutta methods to be overridden
	void rungeKutta();
	virtual std::array<double, 9> f(std::array<double, 9>, std::array<double, 2> &);
	std::array<double, 9> addArrays(std::array<double, 9>, std::array<double, 9>, double);

protected:			 // maybe change to private -> pass by reference
	double h = 0.01; // Time step

	bool reset = false;
	std::array<double, 9> receivedStates; // array to save received drone states
	std::array<double, 2> receivedInputs; // array to save reveiced inputs
	std::array<double, 9> updatedStates;  // calculated new drone states
	std::array<double, 9> K1, K2, K3, K4; // RK4 intermediate steps

	const double g = 9.81;	   // Gravity
	const double C_drag = 0.1; // Drag
	const int m = 3;		   // Mass
};