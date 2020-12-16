#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class PPMImage
{
public:
	PPMImage(void);

	/// destructor, delete image
	~PPMImage(void);

	/// Allocates memory for image
	/// \param width Width of image in pixels
	/// \param height Height of image in pixels
	void AllocateMemory(int width, int height);

	/// Reads in PPM files
	/// \param fileName Name of file to read
	void ReadFile(string fileName);

	/// Outputs PPM file
	/// \param fileName Name of output file produced
	/// \param fileType Type of output file bianry (P6) or ASCII (P3)
	void VerticalFlip();

	/// Verically flips PPM image
	void WriteFile(string fileName, string fileType);

	int width, height;
	unsigned char* image;
private:
	char c, buf[128];
	ifstream file;
	ofstream outFile;
	void CheckComment();
};

