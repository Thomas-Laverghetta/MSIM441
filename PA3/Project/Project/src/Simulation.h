#pragma once

#include <iostream>
#include <gl/glew.h>
#include <gl/glut.h>

class Simulation
{
public:
	Simulation(void);
	~Simulation(void);

	virtual void InitializeSimulation(int argc, char** argv);
	virtual void InitializeGraphics();
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Keyboard(unsigned char key, int x, int y) {}
	virtual void SpecialKey(int key, int x, int y) {}
	virtual void MouseMove(int x, int y) {}
	virtual void PassiveMouseMove(int x, int y) {}
	virtual void Reshape(int w, int h) {}

	void SetWindowPosition(int x, int y);
	void SetWindowSize(int width, int height);
	void SetClock(int timer);
	void SetTitle(char* title) { this->title = title; }
	void Run();

protected: 
	int winWidth;	///< Width of OpenGL window
	int winHeight;	///< Height of OpenGL window
	int positionX, positionY;
	int timer;
	const char * title;

private:
	bool timerSet;
	void RegisterCallbacks();
};

