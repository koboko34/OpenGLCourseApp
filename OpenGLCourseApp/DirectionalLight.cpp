#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.f, -1.f, 0.f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
								GLfloat ambientStrength, GLfloat diffuseStrength,
								GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, ambientStrength, diffuseStrength)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
	GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity); 
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity); 
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
