/// \file
/// MSIM 441 - Programming Assignment One
/// \author Thomas J Laverghetta
/// \date 10/22/2020
///

// include header files
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <cmath>
#include <gl/glew.h>
#include <gl/glut.h>
#include <iomanip>
#include "utility.h"

#define DEBUG

using namespace std;

const double PI = 3.14159265;	/*!< Constant for PI. */

string currentFile;				/*!< Current data file name. */

float* dataset;					/*!< Holds array of dataset values from file.*/
int numDataPoints;				/*!< Number of data point in current data file.*/

///@{
/** min and max data points. */
float minimum, maximum;			
///@}

// Histogram
int numIntervals = 30;			/*!< Number of bins for histogram.*/
float* endPoints = nullptr;		/*!< Array of bin end points.*/
float* prob = nullptr;			/*!< Array of probabilities for each bin.*/
float maxProb = -1.0f;			/*!< Max density of probability.*/

// Theoretical distributions
/* \enum Distributions
* Distribution Curve Types
*/
enum Distributions {
	Null,						/*!< Null value (used at initialization of system)*/
	NORMAL,						/*!< Normal Curve Type */
	EXPO						/*!< Exponetial Curve Type*/
};

Distributions curveType = Null;		/*!< Current curve type. */
int numCurvePoints = 100;			/*!< Number of curve points.*/
float* curveX = new float[numCurvePoints];/*!< Array of X-curve points.*/
float* curveY = new float[numCurvePoints];/*!< Array of Y-curve points.*/

///@{
/** Window size parameters. */
float mu = 0, sigma = 1;	// Normal Distribution
///@}

float lambda = 1;					/*!< Exponential Distribution Rate Parameter. */
float parameterStep = 0.05;			/*!< Step size for annotations. */

///@{
/** Window size parameters. */
int width = 800, height = 600;
///@}

///@{
/** Draw space size parameters. */
float world_x_min, world_x_max, world_y_min, world_y_max;
///@}

///@{
/** Graph axis parameters. */
float axis_x_min, axis_x_max, axis_y_min, axis_y_max;
///@}

/// Normal Distribution Point Generator.
void ComputeNormalFunc()
{
	float stepSize = (maximum - minimum) / ((float)numCurvePoints);

	curveX[0] = minimum;
	curveY[0] = 1.0f / (sigma * sqrt(2.0f * PI)) * exp(-(powf(minimum - mu, 2.0f) / (2.0f * powf(sigma, 2.0f))));

	for (int i = 1; i < numCurvePoints; i++)
	{
		curveX[i] = minimum + i * stepSize;
		curveY[i] = 1.0f / (sigma * sqrt(2.0f * PI)) * exp(-(powf(curveX[i] - mu, 2.0f) / (2.0f * powf(sigma, 2.0f))));
	}
}

/// Exponential Distribution Point Generator.
void ComputeExponentialFunc()
{
	float stepSize = (maximum - minimum) / ((float)numCurvePoints);

	curveX[0] = minimum;
	curveY[0] = 1.0f / lambda * exp(-(minimum) / lambda);

	for (int i = 1; i < numCurvePoints; i++)
	{
		curveX[i] = minimum + i * stepSize;
		curveY[i] = 1.0f / lambda * exp(-(curveX[i]) / lambda);
	}
}

/// Displays Histogram and Theorical Curve to Window.
void Display(void)
{
	/* clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT);

	// Reset Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(1);
	glColor3f(255.0 / 255.0, 255.0 / 255.0, 0.0 / 255.0);

	if (curveType != Distributions::Null) {
		// Draw x and y axes
		glBegin(GL_LINES);
		glVertex2f(axis_x_min, 0);
		glVertex2f(axis_x_max, 0);
		glVertex2f(axis_x_min, 0);
		glVertex2f(axis_x_min, world_y_max * 0.95);
		glEnd();

		stringstream ss;
		ss << maxProb;
		glRasterPos2f(axis_x_min + (maximum - minimum) * 0.02, maxProb - 0.004);
		printString(ss.str());

		// dash to indicate where the max probability is
		glBegin(GL_LINES);
		glVertex2f(axis_x_min - 0.005 * axis_x_max, maxProb);
		glVertex2f(axis_x_min + 0.005 * axis_x_max, maxProb);
		glEnd();


		glRasterPos2f(maximum + (maximum - minimum) * 0.01, maxProb * 0.02);
		printString("Data");

		glRasterPos2f(axis_x_min + (maximum - minimum) * 0.015, maxProb * 1.1);
		printString("Probability Density");


		// Draw Probability
		glColor3f(0.0 / 255.0, 255.0 / 255.0, 0.0 / 255.0);
		glEnable(GL_POLYGON);
		for (int i = 0; i < numIntervals; i++)
			glRectf(endPoints[i], 0, endPoints[i + 1], prob[i]);

		glWindowPos2f(width - 200, height - 100);
		printString("File: " + currentFile);
		glWindowPos2f(width - 200, height - 150);
		printString("Min: " + to_string(minimum));
		glWindowPos2f(width - 200, height - 200);
		printString("Max: " + to_string(maximum));
		glWindowPos2f(width - 200, height - 250);
		printString("Num of Intervals: " + to_string(numIntervals));


		// Draw theoretical distribution
		glColor3f(255.0 / 255.0, 229.0 / 255.0, 204.0 / 255.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(curveX[0], curveY[0]);
		for (int i = 0; i < numCurvePoints; i++) {
			glVertex2f(curveX[i], curveY[i]);
		}
		glEnd();

		glWindowPos2f(width - 200, height - 350);

		string distro;
		if (curveType == Distributions::NORMAL)
			distro = "Normal";
		else if (curveType == Distributions::EXPO)
			distro = "Expo";
		else
			distro = "";
		printString("Distribution: " + distro);

		// string stream variable for setting precision
		stringstream precisionSetter;
		if (curveType == Distributions::NORMAL)
		{
			glWindowPos2f(width - 200, height - 400);
			precisionSetter << fixed << setprecision(2) << mu; // two point precision
			printString("Mu: " + precisionSetter.str());
			precisionSetter.str("");

			glWindowPos2f(width - 200, height - 450);
			precisionSetter << fixed << setprecision(2) << sigma; // two point precision
			printString("Sigma: " + precisionSetter.str());
			precisionSetter.str("");
		}
		else if (curveType == Distributions::EXPO)
		{
			glWindowPos2f(width - 200, height - 400);
			precisionSetter << fixed << setprecision(2) << lambda; // two point precision
			printString("Lambda: " + precisionSetter.str());
			precisionSetter.str("");
		}
	}
	else {
		glRasterPos2f(-0.5,0);
		printString("Please Select Data In Menu (Right Click)");
	}
	glFlush();
	glutSwapBuffers();
}

/// Initialization.
void Init(void)
{
	glClearColor(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/// Computes the Histogram Probabilities.
void ComputeProbability()
{
	// memory management
	if (endPoints != nullptr)
		delete[] endPoints;
	if (prob != nullptr)
		delete[] prob;

	// Initializing max probability
	maxProb = -1.0f;

	// allocating memory for end points and the probabilities
	endPoints = new float[numIntervals + 1];
	prob = new float[numIntervals]();

	// getting the distance between each end point
	float stepSize = (maximum - minimum) / ((float)numIntervals);

	// setting the end points
	endPoints[0] = minimum;
	for (int i = 1; i < numIntervals + 1; i++)
		endPoints[i] = minimum + i * stepSize;

	for (int i = 0; i < numIntervals; i++)
	{
		for (int j = 0; j < numDataPoints; j++)
		{
			// counting number of points within interval
			if (dataset[j] >= endPoints[i] && dataset[j] < endPoints[i + 1])
				prob[i]++;
		}
		prob[i] = prob[i] / ((float)numDataPoints) / stepSize;
		if (prob[i] > maxProb)
			maxProb = prob[i];
	}
	world_y_min = -maxProb * 0.05;
	axis_y_min = 0;
	world_y_max = maxProb * 1.2;
	axis_y_max = maxProb * 1.05;
}

/// Reads Data Files.
void readFile(string fileName)
{
	ifstream inFile(fileName);

	// error checking
	if (!inFile.is_open()) {
		// display error
		cout << "ERROR: NOT ABLE TO OPEN: " << fileName << "\a\n";
		system("pause");
		exit(1);
	}

	// getting number of data points
	inFile >> numDataPoints;

	// Memory Management
	if (dataset != NULL)
		delete[] dataset;
	dataset = new float[numDataPoints];

	// Initialize min, max so to find a actual min, max in data
	minimum = FLT_MAX;
	maximum = -FLT_MAX;

	// Read data and compute minimum and maximum
	for (int i = 0; i < numDataPoints; i++)
	{
		inFile >> dataset[i];
		if (dataset[i] < minimum)
			minimum = dataset[i];
		if (dataset[i] > maximum)
			maximum = dataset[i];
	}

	// Compute the limits for the x-axis and world
	world_x_min = minimum - (maximum - minimum) * 0.1f;
	axis_x_min = minimum - (maximum - minimum) * 0.05f;
	world_x_max = maximum + (maximum - minimum) * 0.1f;
	axis_x_max = maximum + (maximum - minimum) * 0.05f;

	// Compute histogram and limits for the y-axis and world
	ComputeProbability();

	// Compute the theoretical distribution 
	if (curveType == Distributions::NORMAL) {
		// setting mean and sigma to be best 
		mu = 0.0f;
		sigma = 0.0f;
		for (int i = 0; i < numDataPoints; i++)
			mu += dataset[i];
		mu = mu / ((float)numDataPoints);

		for (int i = 0; i < numDataPoints; i++)
			sigma += powf(dataset[i] - mu,2.0f);
		sigma = sigma / ((float)numDataPoints) + parameterStep;

		ComputeNormalFunc();
	}
	else if (curveType == Distributions::EXPO) {
		ComputeExponentialFunc();
	}
}

/// Keyboard Callback Function for Terminating Program.
/// \param key ASCII code of the key pressed.
/// \param x  X coordinate of the mouse cursor when the key is pressed.
/// \param y  Y coordinate of the mouse cursor when the key is pressed.
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q' || key == 'Q')
		exit(0);
}

/// 
/// \param key ASCII code of the key pressed.
/// \param x X coordinate of the mouse cursor when the key is pressed.
/// \param y Y coordinate of the mouse cursor when the key is pressed.
void SpecialKey(int key, int x, int y)	// for the arrow keys
{
	switch (key)
	{
		case GLUT_KEY_UP:
			if (curveType == Distributions::NORMAL)
				sigma += parameterStep;
			else if (curveType == Distributions::EXPO)
				lambda += parameterStep;
			break;

		case GLUT_KEY_DOWN:
			if (curveType == Distributions::NORMAL) {
				sigma -= ((sigma - parameterStep) > 0) * parameterStep;
			}
			else if (curveType == Distributions::EXPO)
				lambda -= ((lambda - parameterStep) > 0) * parameterStep;
			break;

		case GLUT_KEY_LEFT:
			if (curveType == Distributions::NORMAL)
				mu -= parameterStep;
			break;

		case GLUT_KEY_RIGHT:
			if (curveType == Distributions::NORMAL)
				mu += parameterStep;
			break;

		default:
			break;
	}
	
	if (curveType == Distributions::NORMAL)
		ComputeNormalFunc();
	else if (curveType == Distributions::EXPO)
		ComputeExponentialFunc();

	glutPostRedisplay();
}
 
/// Exit Program through Program Menu
/// \param id Exit code 
void TopMenuFunc(int id)
{
	exit(0);
}

/// File menu to choose which data file.
/// \param id Identifies what data file.
void FileMenuFunction(int id)
{
	switch (id)
	{
		case 1:
			curveType = Distributions::NORMAL;
			currentFile = "normal.dat";
			readFile("./Data/normal.dat");
			break;

		case 2:
			curveType = Distributions::EXPO;
			currentFile = "expo.dat";
			readFile("./Data/expo.dat");
			break;

		case 3:
			// first initial is T, therefore, data-file 5
			curveType = Distributions::NORMAL;
			currentFile = "5.dat";
			readFile("./Data/5.dat");
			break;

		case 4:
			// last initial is L, therefore, data-file 15 
			curveType = Distributions::NORMAL;
			currentFile = "15.dat";
			readFile("./Data/15.dat");
			break;

		default:
			break;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
	glutPostRedisplay();
}

/// Distribution menu.
/// \param id Identifies what distribution.
void FuncMenuFunction(int id)
{
	switch (id)
	{
		case 5:
			curveType = Distributions::NORMAL;
			ComputeNormalFunc();
			break;

		case 6:
			curveType = Distributions::EXPO;
			ComputeExponentialFunc();
			break;

		default:
			break;
	}

	glutPostRedisplay();
}

/// Histogram menu to choose number of bins.
/// \param id Identifies number of bins.
void HistogramMenuFunction(int id)
{
	switch (id)
	{
		case 7:
			numIntervals = 30;
			break;

		case 8:
			numIntervals = 40;
			break;

		case 9:
			numIntervals = 50;
			break;

		default:
		break;
	}
	ComputeProbability();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
	glutPostRedisplay();
}

/// Parameter step menu.
/// gives the user the ability to select parameter step size.
/// \param id Identifies parameter step size
void ParameterStepMenuFunction(int id)
{
	switch (id)
	{
		case 10: 
			parameterStep = 0.01;
			break;
		case 11:
			parameterStep = 0.02;
			break;
		case 12:
			parameterStep = 0.05;
			break;
		default:
			break;
	}
}

/// Creates menu for user interface.
void CreateMenu()
{
	int file = glutCreateMenu(FileMenuFunction);
	glutAddMenuEntry("normal.dat", 1);
	glutAddMenuEntry("expo.dat", 2);
	glutAddMenuEntry("5.dat", 3);
	glutAddMenuEntry("15.dat", 4);

	int distribution = glutCreateMenu(FuncMenuFunction);
	glutAddMenuEntry("Normal", 5);
	glutAddMenuEntry("Exponential", 6);

	int histogram = glutCreateMenu(HistogramMenuFunction);
	glutAddMenuEntry("30", 7);
	glutAddMenuEntry("40", 8);
	glutAddMenuEntry("50", 9);

	int parameter = glutCreateMenu(ParameterStepMenuFunction);
	glutAddMenuEntry("0.01", 10);
	glutAddMenuEntry("0.02", 11);
	glutAddMenuEntry("0.05", 12);

	glutCreateMenu(TopMenuFunc);
	glutAddSubMenu("File", file);
	glutAddSubMenu("Distribution", distribution);
	glutAddSubMenu("Histogram", histogram);
	glutAddSubMenu("Parameter Step", parameter);
	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/// Reshapes contain within window when window is reshaped.
/// \param w New window width.
/// \param h New window hight.
void Reshape(int w, int h)
{
	width = w, height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Program Assignent One - creator Thomas Laverghetta");
	glewInit();
	Init();

	// creates the menu callbacks
	CreateMenu();

	// Display Function
	glutDisplayFunc(Display);

	// Reshape Callback
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutMainLoop();
}