#include "TrafficLight.h"


TrafficLight::TrafficLight(void)
{
}


TrafficLight::~TrafficLight(void)
{
}

void TrafficLight::setMaterials()
{
	// In this function, you are supposed to assign values to the variables redOn, redOff,
	// yellowOn, yellowOff, greenOn, greenOff.

	// assigning red light on and off
	redOn = materials["_Red_"];
	redOff = redOn;
	redOff.Kd[0]= 0.25;				// for off state, set red to dim (25% of on)          
	
	// assigning yellow light on and off
	yellowOn = materials["_Yellow_"];
	yellowOff = yellowOn;
	yellowOff.Kd[0] = 0.25;			// off state, set yellow light to 25% of on (dim)
	yellowOff.Kd[1] = 0.25;

	// assigning green light on and off
	greenOn = materials["_Green_"];
	greenOff = greenOn;
	greenOff.Kd[1] = 0.25;			// off state, set green light to 25% of on (dim)

	// initializes lights to off state
	materials["_Red_"] = redOff;
	materials["_Yellow_"] = yellowOff;
	materials["_Green_"] = greenOff;
}

void TrafficLight::setSignal(Signal signal)
{
	// You are supposed to assign the materials used in the ObjModel class based on
	// values of the input signal.

	switch (signal) {
	case Signal::Green:
		materials["_Red_"] = redOff;
		materials["_Yellow_"] = yellowOff;
		materials["_Green_"] = greenOn;
		break;
	case Signal::Yellow:
		materials["_Red_"] = redOff;
		materials["_Yellow_"] = yellowOn;
		materials["_Green_"] = greenOff;
		break;
	default: // red
		materials["_Red_"] = redOn;
		materials["_Yellow_"] = yellowOff;
		materials["_Green_"] = greenOff;
		break;
	}
}

void TrafficLight::ReadFile(string fileName)
{
	// parse obj file
	ObjModel::ReadFile(fileName);

	// save initial red, yellow, and green state
	setMaterials();
}