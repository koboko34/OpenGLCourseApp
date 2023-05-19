#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

#include "CommonValues.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	~Skybox();

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	GLuint uniformView, uniformProjection;
	GLuint textureId;

	Mesh* skyMesh;
	Shader* skyShader;
};

