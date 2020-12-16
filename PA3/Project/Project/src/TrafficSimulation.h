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

	/// Initialization.
	virtual void InitializeSimulation(int argc, char** argv);

	/// Initializes Simulation Graphics, load 3D models, & registers callbacks.
	/// Sets up lighting, generate display lists for the surveillance camera, 
	/// car, and terrain.
	virtual void InitializeGraphics();
	
	/// Updates the dynamic objects.
	/// Update the car position and traffic signals.
	virtual void Update();

	/// Display callback.
	/// Displays 4 viewports.  For for each viewport, set up position and size, projection, 
	/// and camera (ModelView matrix).	
	virtual void Draw();

	/// Update the small viewports' size automatically.
	/// \param w Width of the OpenGL window
	/// \param h Height of the OpenGL window
	virtual void Reshape(int w, int h);

	/// Keyboard callback
	/// Handle regular key presses, and for P3, "r" for reset, "b" for break, "s" for snapshot, and escape for quit.
	virtual void Keyboard(unsigned char key, int x, int y);

	/// Callback function for special keys used to control car simulation.
	/// \param key ASCII code of the key pressed.
	/// \param x X coordinate of the mouse cursor when the key is pressed.
	/// \param y Y coordinate of the mouse cursor when the key is pressed.
	/// Car simulation can be controlled using arrow keys.
	/// Up accelerates, down deceleration, right turns car right, and left turns car left.	
	virtual void SpecialKey(int key, int x, int y);


	virtual void MouseMove(int x, int y);


	virtual void PassiveMouseMove(int x, int y);

	/// Function to draw the entire scene.
	/// This includes, traffic signals (NE, SW, SE, &NW).
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

	Signal NS_Signal = Red;		///< North-South signal.
	Signal WE_Signal = Green;			///< West-East signal.

	Vector3 carPosition = { 2.5, 0, 45 };	///< Car position with initial value.
	Vector3 localCarSpeed = { 0, 0, 0 };	///< Car speed in car's local space.
	Vector3 worldCarSpeed;					///< Car speed in world space.

	float carDirection = 180;	///< Car direction
	string carHeading = "N";	///< String for car heading

	Vector3	localCameraOffset = { 0, 2, -6 };	///< Third person camera offset in the car's local space.
	Vector3 worldCameraOffset = { 0, 2, 6 };	///< Third person camera offset in world space. //// KBTs

	int sWidth;		///< Width of the small viewport
	int sHeight;	///< Height of the small viewport
};

