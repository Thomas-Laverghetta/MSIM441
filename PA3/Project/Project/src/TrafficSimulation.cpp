#include "TrafficSimulation.h"

/// Initialization.
void TrafficSimulation::InitializeSimulation(int argc, char** argv)
{
	Simulation::InitializeSimulation(argc, argv);
}

/// Initializes Simulation Graphics, load 3D models, & registers callbacks.
/// Sets up lighting, generate display lists for the surveillance camera, 
/// car, and terrain.
void TrafficSimulation::InitializeGraphics()
{
	
	Simulation::InitializeGraphics();

	// Load the 3D models.
	trafficLight.ReadFile("Models/TrafficLight.obj");	// traffic lights
	car.ReadFile("Models/taxi.obj");					// taxi
	surveillanceCamera.ReadFile("Models/camera.obj");	// surveillance cameras

	// billboards
	bb1.ReadFile("Billboard_Images/puppy_monkey_baby.ppm");
	bb2.ReadFile("Billboard_Images/dog.ppm");
	
	glClearColor(0.5, 0.5, 1.0, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Note that the light is defined in the eye or camera frame.
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat ambient[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Generate display list for the surveillance camera.
	surveillanceCameraID = glGenLists(1);

	glNewList(surveillanceCameraID, GL_COMPILE);
	surveillanceCamera.Draw();
	glEndList();

	// Generate display list for the car.
	carID = glGenLists(1);

	glNewList(carID, GL_COMPILE);
	car.Draw();
	glEndList();

	// Generate the display list for terrain, including road and grass.
	terrainID = glGenLists(1);
	glNewList(terrainID, GL_COMPILE);
	glDisable(GL_LIGHTING);

	// Grass
	glColor3f(0, 0.7, 0);
	glBegin(GL_QUADS);
	glVertex3f(-1000, 0, 1000);
	glVertex3f(-10, 0, 1000);
	glVertex3f(-10, 0, 10);
	glVertex3f(-1000, 0, 10);

	glVertex3f(10, 0, 1000);
	glVertex3f(1000, 0, 1000);
	glVertex3f(1000, 0, 10);
	glVertex3f(10, 0, 10);

	glVertex3f(10, 0, -10);
	glVertex3f(1000, 0, -10);
	glVertex3f(1000, 0, -1000);
	glVertex3f(10, 0, -1000);

	glVertex3f(-1000, 0, -10);
	glVertex3f(-10, 0, -10);
	glVertex3f(-10, 0, -1000);
	glVertex3f(-1000, 0, -1000);
	glEnd();

	// Roads
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.2);

	glVertex3f(-10, 0, 1000);
	glVertex3f(10, 0, 1000);
	glVertex3f(10, 0, -1000);
	glVertex3f(-10, 0, -1000);

	glVertex3f(-1000, 0, 10);
	glVertex3f(1000, 0, 10);
	glVertex3f(1000, 0, -10);
	glVertex3f(-1000, 0, -10);
	glEnd();

	// Yellow line
	glBegin(GL_QUADS);
	// yellow color
	glColor3f(1, 1, 0);

	// South Direction
	glVertex3f(-0.1, 0.01, 1000);
	glVertex3f(0.1, 0.01, 1000);
	glVertex3f(0.1, 0.01, 10);
	glVertex3f(-0.1, 0.01, 10);

	// North Direction
	glVertex3f(-0.1, 0.01, -1000);
	glVertex3f(0.1, 0.01, -1000);
	glVertex3f(0.1, 0.01, -10);
	glVertex3f(-0.1, 0.01, -10);

	// West direction
	glVertex3f(-1000, 0.01, -0.1);
	glVertex3f(-1000, 0.01, 0.1);
	glVertex3f(-10, 0.01, -0.1);
	glVertex3f(-10, 0.01, 0.1);

	// East Direction
	glVertex3f(1000, 0.01, -0.1);
	glVertex3f(1000, 0.01, 0.1);
	glVertex3f(10, 0.01, -0.1);
	glVertex3f(10, 0.01, 0.1);

	glEnd();

	// Dashed White lines (EXTRA CREDIT)
	// quad segments are 3m long w/width 0.2m w/9m gaps
	// dimensions were gathered from US Dept Transportation:
	// http://mutcd.fhwa.dot.gov/htm/2003r1r2/part3/part3a.htm#section3A05
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	for (int i = 10; i < 1000; i += 9)
	{
		glVertex3f(4.9, 0.01, i + 3);
		glVertex3f(5.1, 0.01, i + 3);
		glVertex3f(4.9, 0.01, i);
		glVertex3f(5.1, 0.01, i);

		glVertex3f(i + 3, 0.01, 4.9);
		glVertex3f(i + 3, 0.01, 5.1);
		glVertex3f(i, 0.01, 4.9);
		glVertex3f(i, 0.01, 5.1);

		glVertex3f(-4.9, 0.01, i + 3);
		glVertex3f(-5.1, 0.01, i + 3);
		glVertex3f(-4.9, 0.01, i);
		glVertex3f(-5.1, 0.01, i);

		glVertex3f(i + 3, 0.01, -4.9);
		glVertex3f(i + 3, 0.01, -5.1);
		glVertex3f(i, 0.01, -4.9);
		glVertex3f(i, 0.01, -5.1);
	}

	for (int i = -10; i > -1000; i -= 9)
	{
		glVertex3f(4.9, 0.01, i - 3);
		glVertex3f(5.1, 0.01, i - 3);
		glVertex3f(4.9, 0.01, i);
		glVertex3f(5.1, 0.01, i);

		glVertex3f(i - 3, 0.01, 4.9);
		glVertex3f(i - 3, 0.01, 5.1);
		glVertex3f(i, 0.01, 4.9);
		glVertex3f(i, 0.01, 5.1);

		glVertex3f(-4.9, 0.01, i - 3);
		glVertex3f(-5.1, 0.01, i - 3);
		glVertex3f(-4.9, 0.01, i);
		glVertex3f(-5.1, 0.01, i);

		glVertex3f(i - 3, 0.01, -4.9);
		glVertex3f(i - 3, 0.01, -5.1);
		glVertex3f(i, 0.01, -4.9);
		glVertex3f(i, 0.01, -5.1);
	}
	glEnd();

	glEndList();

	// placing and orienting billboards
	bb1.SetLocation({ 20, 10, -30 });
	bb1.SetSize(10, 5);
	bb1.SetOrientation(-30);

	bb2.SetLocation({ -20, 10, -30 });
	bb2.SetSize(10, 5);
	bb2.SetOrientation(30);

	// Setting Timer for update function
	Simulation::SetClock(updateInterval);
}

/// Update the small viewports' size automatically.
/// \param w Width of the OpenGL window
/// \param h Height of the OpenGL window
void TrafficSimulation::Reshape(int w, int h)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0.0, 50.0, 0.0, 50.0);

	//glViewport(0, 0, w, h);

	winWidth = w, winHeight = h;

	// Update sWidth and sHeight here.
	sWidth = (winWidth - 60) / 3;
	sHeight = winHeight / 4;
}

/// Updates the dynamic objects.
/// Update the car position and traffic signals.
void TrafficSimulation::Update()
{

	// Update car position. Simple Euler (delta time * speed)
	carPosition.x += updateInterval * worldCarSpeed.x;
	carPosition.y += updateInterval * worldCarSpeed.y;
	carPosition.z += updateInterval * worldCarSpeed.z;

	// State machine for the traffic signals using three variables: NS_Signal, WE_Signal, and counter.
	counter++;
	cout << counter << endl;
	if (NS_Signal == Signal::Green && counter >= (5000 / updateInterval))
	{
		NS_Signal = Signal::Yellow;
		WE_Signal = Signal::Red;
		counter = 0;
	}
	else if (NS_Signal == Signal::Yellow && counter >= (1000 / updateInterval))
	{
		NS_Signal = Signal::Red;
		WE_Signal = Signal::Green;
		counter = 0;
	}
	else if (WE_Signal == Signal::Green && counter >= (5000 / updateInterval))
	{
		WE_Signal = Signal::Yellow;
		NS_Signal = Signal::Red;
		counter = 0;
	}
	else if (WE_Signal == Signal::Yellow && counter >= (1000 / updateInterval))
	{
		WE_Signal = Signal::Red;
		NS_Signal = Signal::Green;
		counter = 0;
	}
}

/// Keyboard callback
/// Handle regular key presses, and for P2, "r" for reset, "b" for break, and escape for quit.
void TrafficSimulation::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r': // resetting car dynamic
		// resetting car position and velocity
		carPosition = { 3, 0, 45 };
		localCarSpeed = { 0, 0, 0 };
		worldCarSpeed = { 0, 0, 0 };

		// resetting car direction (heading)
		carDirection = 180;
		carHeading = "N";
		break;

	case 'b': // Stops car immediately
		localCarSpeed = { 0, 0, 0 };
		worldCarSpeed = { 0, 0, 0 };
		break;

	//case 's':
	//	screenshot.AllocateMemory(winWidth, winHeight);
	//	glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenshot.image);
	//	screenshot.VerticalFlip();
	//	screenshot.WriteFile("Screenshot.ppm", "P3");
	//	break;

	case 27: // escape
		exit(0);
		break;
	}

	glutPostRedisplay();
}

/// Callback function for special keys used to control car simulation.
/// \param key ASCII code of the key pressed.
/// \param x X coordinate of the mouse cursor when the key is pressed.
/// \param y Y coordinate of the mouse cursor when the key is pressed.
/// Car simulation can be controlled using arrow keys.
/// Up accelerates, down deceleration, right turns car right, and left turns car left.
void TrafficSimulation::SpecialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		// Update car direction
		carDirection += 0.5;

		// reset heading once a full circle has been acheived
		carDirection = carDirection - (carDirection > 360) * 360;

		// Compute new car heading to display
		if (carDirection > 337.5 || carDirection < 22.5)
			carHeading = "S";
		else if (carDirection > 22.5 && carDirection < 67.5)
			carHeading = "SE";
		else if (carDirection > 67.5 && carDirection < 112.5)
			carHeading = "E";
		else if (carDirection > 112.5 && carDirection < 157.5)
			carHeading = "NE";
		else if (carDirection > 157.5 && carDirection < 202.5)
			carHeading = "N";
		else if (carDirection > 202.5 && carDirection < 247.5)
			carHeading = "NW";
		else if (carDirection > 247.5 && carDirection < 292.5)
			carHeading = "W";
		else if (carDirection > 292.5 && carDirection < 337.5)
			carHeading = "SW";

		// Update the third person camera offset in the world frame.
		worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);

		// update world car speed
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		break;
	case GLUT_KEY_RIGHT:
		// Update car direction
		carDirection -= 0.5;

		// reset heading once a full circle has been acheived
		carDirection = carDirection - (carDirection > 360) * 360;

		// Compute new car heading to display.
		if (carDirection > 337.5 || carDirection < 22.5)
			carHeading = "S";
		else if (carDirection > 22.5 && carDirection < 67.5)
			carHeading = "SE";
		else if (carDirection > 67.5 && carDirection < 112.5)
			carHeading = "E";
		else if (carDirection > 112.5 && carDirection < 157.5)
			carHeading = "NE";
		else if (carDirection > 157.5 && carDirection < 202.5)
			carHeading = "N";
		else if (carDirection > 202.5 && carDirection < 247.5)
			carHeading = "NW";
		else if (carDirection > 247.5 && carDirection < 292.5)
			carHeading = "W";
		else if (carDirection > 292.5 && carDirection < 337.5)
			carHeading = "SW";

		// Update the third person camera offset in the world frame.
		worldCameraOffset = computeRotatedVector(localCameraOffset, carDirection);

		// update world car speed
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		break;
	case GLUT_KEY_UP:
		// acceleration
		// Honda S2000 max speed is ~145 mph
		localCarSpeed.z = localCarSpeed.z + 0.05 * (localCarSpeed.z < 1.3);

		// calculating the car's world speed
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		break;
	case GLUT_KEY_DOWN:
		// deceleration
		// onda S2000 max reverse speed is ~56mph
		localCarSpeed.z = localCarSpeed.z - 0.05 * (localCarSpeed.z > -0.4);
		worldCarSpeed = computeRotatedVector(localCarSpeed, carDirection);
		break;
	}
}

void TrafficSimulation::MouseMove(int x, int y)
{
}

void TrafficSimulation::PassiveMouseMove(int x, int y)
{
}

/// Function to draw the entire scene.
/// This includes, traffic signals (NE, SW, SE, &NW).
void TrafficSimulation::drawScene()
{
	// Draw terrain
	glCallList(terrainID);

	glEnable(GL_LIGHTING);

	// North-East (NS_Signal)
	glPushMatrix();
	glTranslatef(10, 0, -10.5);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, -10);
	glRotatef(-45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South-West (NS_Signal)
	glPushMatrix();
	glTranslatef(-10.5, 0, 10);
	glRotatef(180, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(NS_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, 10);
	glRotatef(90, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// South-East (WE_Signal)
	glPushMatrix();
	glTranslatef(10.5, 0, 10);
	glRotatef(-90, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 10);
	glRotatef(-135, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// North-West (WE_Signal)
	glPushMatrix();
	glTranslatef(-10, 0, -10.5);
	glRotatef(90, 0, 1, 0);
	glScalef(1 / 3.28 / 12, 1 / 3.28 / 12, 1 / 3.28 / 12);
	trafficLight.setSignal(WE_Signal);
	trafficLight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, -10);
	glRotatef(45, 0, 1, 0);
	glCallList(surveillanceCameraID);
	glPopMatrix();

	// Draw the car.
	glPushMatrix();
	glTranslatef(carPosition.x, carPosition.y, carPosition.z);
	glRotatef(carDirection, 0, 1, 0);
	car.Draw();
	glCallList(carID);
	glPopMatrix();

	// draw billboards
	bb1.Draw();
	bb2.Draw();
}

/// Display callback.
/// Displays 4 viewports.  For for each viewport, set up position and size, projection, 
/// and camera (ModelView matrix).
void TrafficSimulation::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Generate head-up display (HUD), Heading & Speed
	stringstream s_spd;
	float speed = localCarSpeed.z / ((float)updateInterval) * 1000.0f * 2.23694; // m/ms to mph
	if (speed < 0)
		speed = -1.0f * speed;
	s_spd << fixed << setprecision(0) << speed; // if speed is negative, display has positive

	glColor3f(1, 1, 0);
	glWindowPos2f(winWidth / 2.0 - 105, winHeight - sHeight - 40);
	printLargeString("Heading: ");
	printLargeString(carHeading);
	printLargeString("     ");
	printLargeString("Speed: ");
	printLargeString(s_spd.str());
	printLargeString(" mph");

	//// Setup viewport, projection, and camera for the main view.
	glViewport(0, 0, winWidth, winHeight - sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)winWidth / (winHeight - sHeight - 15), 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Car Third Person Viewer
	gluLookAt(carPosition.x + worldCameraOffset.x, carPosition.y + worldCameraOffset.y, carPosition.z + worldCameraOffset.z,
		carPosition.x, carPosition.y + 1.5, carPosition.z, 0, 1, 0);
	drawScene();

	// Setup viewport, projection, and camera for the South-East camera and draw the scene again.
	glViewport(winWidth - 3 * sWidth - 45, winHeight - sHeight - 15, sWidth, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)winWidth / (winHeight - sHeight - 15), 1, 1000);
	gluLookAt(10, 3, 10, -3, 3, -10, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// diplsay the camera name
	glColor3f(1, 1, 0);
	glWindowPos2f(winWidth - 3 * sWidth - 45, winHeight - sHeight - 15);
	printLargeString("South-East Camera");
	drawScene();

	// Setup the viewport, projection, camera for the top view and draw the scene again.
	glViewport(winWidth - 2 * sWidth - 30, winHeight - sHeight - 15, sWidth, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)winWidth / (winHeight - sHeight - 15), 1, 1000);
	gluLookAt(0, 50, 0, 0, 0, 0, 0, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawScene();

	// Setup viewport, projection, camera for the South-West camera and draw the scene again.
	glViewport(winWidth - sWidth - 15, winHeight - sHeight - 15, sWidth, sHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)winWidth / (winHeight - sHeight - 15), 1, 1000);
	gluLookAt(-10, 3, 10, 10, 3, -3, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// display the camera name
	glColor3f(1, 1, 0);
	glWindowPos2f(winWidth - sWidth - 15, winHeight - sHeight - 15);
	printLargeString("South-West Camera");
	drawScene();

	glutSwapBuffers();
	glFlush();
}