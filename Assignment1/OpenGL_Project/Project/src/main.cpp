/*

	MSIM441 - Introduction to Visulization and Graphics
	Instructor: Dr. Shen
	Student: Thomas J Laverghetta, CMSE UG Student
	Date: 10/22/2020

*/

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

const double PI = 3.14159265;

// Files names
char* fileName1, * fileName2;
string currentFile;

// Input Data
float* dataset;
int numDataPoints;
float minimum, maximum;

// Histogram
int numIntervals = 30;
float* endPoints = new float[numIntervals + 1];
float* prob = new float[numIntervals];
float maxProb = -1;

// Theoretical distributions
int curveType = 0;
int numCurvePoints = 100;
float* curveX = new float[numCurvePoints];
float* curveY = new float[numCurvePoints];

// Parameters
float mu = 0, sigma = 1;	// Normal Distribution
float lambda = 1;			// Exponential Distribution
float parameterStep = 0.05;	// Step size for changing parameter values

// Drawing parameters
int width = 800, height = 600;
float world_x_min, world_x_max, world_y_min, world_y_max;
float axis_x_min, axis_x_max, axis_y_min, axis_y_max;

// Compute all points for normal distribution
void ComputeNormalFunc(float mu, float sigma)
{
	float stepSize = (maximum - minimum) / numCurvePoints;

	curveX[0] = minimum;
	curveY[0] = 1 / (sigma * sqrt(2 * PI)) * exp(-(powf(minimum - mu, 2) / (2 * powf(sigma, 2))));

	for (int i = 1; i < numCurvePoints; i++)
	{
		curveX[i] = minimum + i * stepSize;
		curveY[i] = 1 / (sigma * sqrt(2 * PI)) * exp(-(powf(curveX[i] - mu, 2) / (2 * powf(sigma, 2))));
	}
}

// Compute all points for exponential distribution
void ComputeExponentialFunc(float lambda)
{
	float stepSize = (maximum - minimum) / numCurvePoints;

	curveX[0] = minimum;
	curveY[0] = 1 / lambda * exp(-(minimum) / lambda);

	for (int i = 1; i < numCurvePoints; i++)
	{
		curveX[i] = minimum + i * stepSize;
		curveY[i] = 1/ lambda * exp(-(curveX[i]) / lambda);
	}
}

// redisplay function for glut function callback
void Display(void)
{
	/* clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT);

	// Reset Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(1);
	glColor3f(1.0, 1.0, 1.0);

	// Draw x and y axes
	glBegin(GL_LINES);
	glVertex2f(axis_x_min, 0);
	glVertex2f(axis_x_max, 0);
	glVertex2f(axis_x_min, 0);
	glVertex2f(axis_x_min, world_y_max * 0.95);
	glEnd();

	stringstream ss;
	ss << maxProb;
	glRasterPos2f(axis_x_min + (maximum - minimum) * 0.02, maxProb-0.004);
	printString(ss.str());

	// dash to indicate where the max probability is
	glBegin(GL_LINES);
	glVertex2f(axis_x_min - 0.06, maxProb);
	glVertex2f(axis_x_min + 0.06, maxProb);
	glEnd();


	glRasterPos2f(maximum + (maximum - minimum) * 0.01, maxProb * 0.02);
	printString("Data");

	glRasterPos2f(axis_x_min + (maximum - minimum) * 0.015, maxProb * 1.1);
	printString("Probability Density");


	// Draw Probability
	glColor3f(0.0, 1.0, 0.0);
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
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(curveX[0], curveY[0]);
	for (int i = 0; i < numCurvePoints; i++)
	{
		glVertex2f(curveX[i], curveY[i]);
	}
	glEnd();
	
	glWindowPos2f(width - 200, height - 350);

	string distro;
	if (curveType == 1)
		distro = "Normal";
	else if (curveType == 2)
		distro = "Expo";
	else
		distro = "";
	printString("Distribution: " + distro);

	// string stream variable for setting precision
	stringstream precisionSetter;
	if (curveType == 1)
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
	else if (curveType == 2)
	{
		glWindowPos2f(width - 200, height - 400);
		precisionSetter << fixed << setprecision(2) << lambda; // two point precision
		printString("Lambda: " + precisionSetter.str());
		precisionSetter.str("");
	}

	glFlush();
	glutSwapBuffers();
}

void Init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Compute the probability for the histogram (vertical axis)
void ComputeProbability(int numIntervals)
{
	if (endPoints != NULL)
		delete[] endPoints;

	if (prob != NULL)
		delete[] prob;

	maxProb = -1;

	endPoints = new float[numIntervals + 1];
	prob = new float[numIntervals]();

	float stepSize = (maximum - minimum) / numIntervals;

	endPoints[0] = minimum;
	for (int i = 1; i < numIntervals + 1; i++)
	{
		endPoints[i] = minimum + i * stepSize;
	}

	for (int i = 0; i < numIntervals; i++)
	{
		for (int j = 0; j < numDataPoints; j++)
		{
			if (dataset[j] >= endPoints[i] && dataset[j] < endPoints[i + 1])
			{
				prob[i]++;
			}
		}
		prob[i] = prob[i] / numDataPoints / stepSize;
		if (prob[i] > maxProb)
			maxProb = prob[i];
	}

	world_y_min = -maxProb * 0.05;
	axis_y_min = 0;
	world_y_max = maxProb * 1.2;
	axis_y_max = maxProb * 1.05;
}

void readFile(string fileName)
{
	ifstream inFile(fileName);
	if (!inFile.is_open())
	{
		cout << fileName << " couldn't be opened.\n";
		system("pause");
		exit(1);
	}

	inFile >> numDataPoints;

	// Memory Management
	if (dataset != NULL)
		delete[] dataset;
	dataset = new float[numDataPoints];

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
	world_x_min = minimum - (maximum - minimum) * 0.1;
	axis_x_min = minimum - (maximum - minimum) * 0.05;
	world_x_max = maximum + (maximum - minimum) * 0.1;
	axis_x_max = maximum + (maximum - minimum) * 0.05;

	// Compute histogram and limits for the y-axis and world
	ComputeProbability(numIntervals);

	// Compute the theoretical distribution 
	if (curveType == 1)
		ComputeNormalFunc(mu, sigma);
	else if (curveType == 2)
		ComputeExponentialFunc(lambda);
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q' || key == 'Q')
		exit(0);
}

void SpecialKey(int key, int x, int y)	// for the arrow keys
{
	switch (key)
	{
		case GLUT_KEY_UP:
			if (curveType == 1)
				sigma += parameterStep;
			else if (curveType == 2)
				lambda += parameterStep;
			break;

		case GLUT_KEY_DOWN:
			if (curveType == 1)
				sigma -= parameterStep;
			else if (curveType == 2)
				lambda -= parameterStep;
			break;

		case GLUT_KEY_LEFT:
			if (curveType == 1)
				mu -= parameterStep;
			break;

		case GLUT_KEY_RIGHT:
			if (curveType == 1)
				mu += parameterStep;
			break;

		default:
			break;
	}
	
	if (curveType == 1)
		ComputeNormalFunc(mu, sigma);
	else if (curveType == 2)
		ComputeExponentialFunc(lambda);

	glutPostRedisplay();
}

void TopMenuFunc(int id)
{
	exit(0);
}

void FileMenuFunction(int id)
{
	switch (id)
	{
		case 1:
			curveType = 1;
			currentFile = "normal.dat";
			readFile("./Data/normal.dat");
			break;

		case 2:
			curveType = 2;
			currentFile = "expo.dat";
			readFile("./Data/expo.dat");
			break;

		case 3:
			curveType = 2;
			currentFile = "2.dat";
			readFile("./Data/2.dat");
			break;

		case 4:
			curveType = 2;
			currentFile = "11.dat";
			readFile("./Data/11.dat");
			break;

		default:
			break;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
	glutPostRedisplay();
}

void FuncMenuFunction(int id)
{
	switch (id)
	{
		case 5:
			curveType = 1;
			ComputeNormalFunc(mu, sigma);
			break;

		case 6:
			curveType = 2;
			ComputeExponentialFunc(lambda);
			break;

		default:
			break;
	}

	glutPostRedisplay();
}

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
	ComputeProbability(numIntervals);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
	glutPostRedisplay();
}


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

// menu options
void CreateMenu()
{
	int file = glutCreateMenu(FileMenuFunction);
	glutAddMenuEntry("normal.dat", 1);
	glutAddMenuEntry("expo.dat", 2);
	glutAddMenuEntry("2.dat", 3);
	glutAddMenuEntry("11.dat", 4);

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


	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutMainLoop();
}