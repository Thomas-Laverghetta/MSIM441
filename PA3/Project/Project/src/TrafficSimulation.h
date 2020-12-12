#pragma once

#include "simulation.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include <gl/glew.h>
#include <gl/glut.h>

#include "ObjModel.h"
#include "TrafficLight.h"
#include "utility.h"
#include "BillBoard.h"


class TrafficSimulation : public Simulation
{
public:
	TrafficSimulation(void) {}
	~TrafficSimulation(void) {}
	virtual void InitializeSimulation(int argc, char** argv);
	virtual void InitializeGraphics();
	virtual void Update();
	virtual void Draw();
	virtual void Reshape(int w, int h);
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void SpecialKey(int key, int x, int y);
	virtual void MouseMove(int x, int y);
	virtual void PassiveMouseMove(int x, int y);
	void drawScene();
private:
	int counter = 0;			///< Counter for the traffic signal.  Only one is needed.
	int updateInterval = 20;	///< Update interval for the update function in miliseconds.

	Billboard bb1;					///< First Billboard
	Billboard bb2;					///< second billboard	
	ObjModel car;					///< Object model for car
	ObjModel surveillanceCamera;	///< Object model for surveillance cameras
	TrafficLight trafficLight;		///< Object model for traffic lights

	int carID;						///< Display List ID for car
	int surveillanceCameraID;		///< Display list ID for surveillance camera
	int terrainID;					///< Display list ID for terrain

	Signal NS_Signal = Green;		///< North-South signal.
	Signal WE_Signal = Red;			///< West-East signal.

	Vector3 carPosition = { 2.5, 0, 45 };	///< Car position with initial value.
	Vector3 localCarSpeed = { 0, 0, 0 };	///< Car speed in car's local space.
	Vector3 worldCarSpeed;					///< Car speed in world space.

	float carDirection = 180;	///< Car direction
	string carHeading = "N";	///< String for car heading

	Vector3	localCameraOffset = { 0, 2, -6 };	///< Third person camera offset in the car's local space.
	Vector3 worldCameraOffset = { 0, 2, 6 };	///< Third person camera offset in world space. //// KBTs

	int winWidth;	///< Width of OpenGL window
	int winHeight;	///< Height of OpenGL window
	int sWidth;		///< Width of the small viewport
	int sHeight;	///< Height of the small viewport
};

