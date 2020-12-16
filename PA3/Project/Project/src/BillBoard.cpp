#include "Billboard.h"


Billboard::Billboard(void)
{
}

Billboard::~Billboard(void)
{
}

void Billboard::ReadFile(string fileName)
{
	// Read the texture file, generate the texture object, and configure
	// the texturing parameters.

	// reading in image
	textureImage.ReadFile(fileName);

	/// referenced from ObjModel::ReadFile
	glGenTextures(1, &textureNumber);									// creating texture reference
	glBindTexture(GL_TEXTURE_2D, textureNumber);						// blinding the texture to the target (2d texture) - subsequent operations will effect texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);			// 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.image);
}

void Billboard::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void Billboard::SetLocation(Vector3 location) { this->location = location; }

void Billboard::SetOrientation(float orientation)
{
	this->orientation = orientation;
}

void Billboard::Draw()
{	
	// Draw the board and pillar.  Use texture mapping for the board only.
	// The pillar is drawn just using simple geometry, i.e., a rectangle.

	//--------------------------TEXTURE MAPPING-----------------------

	// gotten from ObjModel DrawMaterials
	glEnable(GL_TEXTURE_2D);						// 
	glBindTexture(GL_TEXTURE_2D, textureNumber);	// binding to texture created
	glDisable(GL_LIGHTING);							// disabling gl lighting (might be enabled from previous code execution)
	glMatrixMode(GL_MODELVIEW);						// applying subsquent matrix operations to the modelview matrix stack

	glPushMatrix();

	// scale then rotate then translate the billboard
	glTranslatef(location.x, location.y, location.z);	
	glRotatef(orientation, 0, 1, 0);
	glScalef(width, height, 1);

	// billboard quad to map too
	glBegin(GL_QUADS);
		// pointing face in z-direction
		glNormal3f(0, 0, 1);		

		// setting coodinates for texture and billboard
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	//--------------------------PILLAR-----------------------
	glPushMatrix();									
	glTranslatef(location.x, 0, location.z);		// setting to same location as billboard
	glScalef(0.25, location.y, 0.25);				// scaling the square to be a pole

	// white pillar
	glColor3f(1, 1, 1);			

	// quad pillar 
	glBegin(GL_QUADS);
		// pointing face in z-direction
		glNormal3f(0, 0, 1);		

		// creating simple square 
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
	glPopMatrix();
}
