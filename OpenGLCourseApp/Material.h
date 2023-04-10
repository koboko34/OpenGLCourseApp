#pragma once

#include <GL/glew.h>

class Material
{
public:

	Material();
	Material(GLfloat specularStrength, GLfloat shininessStrength);
	~Material();

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:

	GLfloat specularIntensity;
	GLfloat shininess;
};

