#include "PPMImage.h"


PPMImage::PPMImage(void)
{
	image = NULL;
}

PPMImage::~PPMImage(void)
{
	if (image)
		delete [] image;
}

void PPMImage::AllocateMemory(int width, int height)
{
	if (image)
		delete[] image;

	this->width = width;
	this->height = height;
	image = new unsigned char [width * height * 3];
}

void PPMImage::ReadFile(string fileName)
{
	if (image)
		delete[] image;

	file.open(fileName, ios_base::binary);

	if (!file.is_open())
	{
		cout << fileName << " couldn't be opened.\n";
		system("pause");
		exit(1);
	}
	
	string fileType;
	file >> fileType;

	CheckComment();

	// dimensions of image
	file >> width >> height;
	image = new unsigned char [width * height * 3];

	int maxValue;

	file >> maxValue;

	// read in data
	if (fileType == "P3")
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				for (int k = 0; k < 3; k++)
				{
					// image elements equal the data from ppm
					string temp;
					file >> temp;

					// reading in each pixel
					image[(i * width + j) * 3 + k] = (unsigned char) atoi(temp.c_str());
				}
	}

	else if (fileType == "P6")
	{
		file.getline(buf, 127);
		file.read((char *) image, height * width * 3);
	}

	file.close();
}

void PPMImage::WriteFile(string fileName, string fileType)
{
	// Read and understand the code.
	if (fileType == "P6")
	{
		outFile = ofstream(fileName, ios_base::binary);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}
	
		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		outFile.write((char *) image, height * width * 3);
	}
	else if (fileType == "P3")
	{
		outFile = ofstream(fileName);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}
	
		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				for (int k = 0; k < 3; k++)
				{
					outFile << (int) image[(i * width + j) * 3 + k] << " ";
				}
			outFile << endl;
		}
	}

	outFile.close();
}

void PPMImage::CheckComment()
{
	c = file.peek();

	while (c == '#' || c == '\n' || c == ' ')
	{
		file.getline(buf, 127);
		c = file.peek();
	}
}

void PPMImage::VerticalFlip()
{
	// Do a vertical flip.  You need to use the help variable declared above.
	// Also do dynamic memory allocation for the variable based on the image size.

	// creating tmp image to hold flipped image
	unsigned char* flippedImage = new unsigned char[width * height * 3];
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			for (int k = 0; k < 3; k++)
			{
				flippedImage[(i * width + j) * 3 + k] = image[((height - 1 - i) * width + j) * 3 + k];
			}
	}

	delete[] image;
	image = flippedImage;
}