#pragma once

#include <iostream>
#include <string>
#include <gl/glut.h>

#include "PPMImage.h";
#include "Vector3D.h";

using namespace std;

class Billboard
{
public:
	Billboard(void);
	~Billboard(void);

	/// Reads in billboard image and creates texture from image
	/// \param fileName Name of PPM billboard image
	void ReadFile(string fileName);

	/// Sets width and height of billboard
	/// \param width Width of billboard sign
	/// \param height Height of billboard sign
	void SetSize(float width, float height);

	/// Sets the location of the billboard in world space
	/// \param location Location billboard
	void SetLocation(Vector3 location);

	/// Sets the orientation of the billboard (angle)
	/// \param orientation Orientation of billboard
	void SetOrientation(float orientation);

	/// Will construct a billboard and pillar and draw to OpenGL
	void Draw();

private:
	PPMImage textureImage;
	float width, height;
	// The variable location contains 3 variables: x, y, z, where y is really the elevation of the board.
	Vector3 location;
	float orientation;
	GLuint textureNumber;
};

