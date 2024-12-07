#pragma once
#include <array>
#include <vector>

class DroneController
{
public:
	DroneController();			// constructor
	virtual ~DroneController(); // destructor

	// get and set methods
	std::array<double, 2> getReceivedVelref();
	void setReceivedVelref(std::array<double, 2>);

	std::array<double, 2> getInput();
	void setInput(std::array<double, 2>);

	std::array<double, 9> getReceivedUpdatedStates();
	void setReceivedUpdatedStates(std::array<double, 9>);

	double getThetaref();
	void setThetaref(double);

	double getFrefy();
	void setFrefy(double);

	void setDroneType(bool);

	// methods for the controller calculations
	double calculate_d_osc(int, std::array<double, 9>);
	double calculate_F_refx(int, int, double, std::array<double, 2>, std::array<double, 9>, double);
	double calculate_F_refy(int, int, double, std::array<double, 2>, std::array<double, 9>);
	double calculate_theta_ref(double, double);
	double calculate_thrust(double, std::array<double, 9>);
	double calculate_omega(double, double, std::array<double, 9>);

	// calculate the Inputs for the drone dynamics
	void calculateControllerOutput();

protected:			  // maybe change to private -> pass by reference
	double hc = 0.01; // Time step
	bool droneType;

	std::array<double, 2> input;
	std::array<double, 2> receivedVel_ref; // array to save inputs
	std::array<double, 9> updatedStates;   // current drone states

	double d_osc;
	double F_refx;
	double F_refy;
	double theta_ref;

	// constant parameters
	const int D_osc = 50;
	const double tx = 0.2;
	const double ty = 0.2;
	const double t_theta = 0.1;
	const double g = 9.81;
	const int md = 3;
	const int mc = 2;
};