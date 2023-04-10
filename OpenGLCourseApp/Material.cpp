#include "Material.h"

Material::Material()
{
	specularIntensity = 0.f;
	shininess = 0.f;
}

Material::Material(GLfloat specularStrength, GLfloat shininessStrength)
{
	specularIntensity = specularStrength;
	shininess = shininessStrength;
}

Material::~Material()
{
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}
