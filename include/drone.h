#pragma once
#include <array>

// base drone class
class Drone
{
public:
	Drone(double, double); // constructor
	virtual ~Drone();	   // destructor

	// get and set methods
	std::array<double, 9> getStates();
	void setStates(std::array<double, 9>);

	bool getReset();
	void setReset(bool);

private:
	bool reset = false;
	std::array<double, 9> droneStates; // drone states
};
