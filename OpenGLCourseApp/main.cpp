#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;

Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	GLfloat vertices[] = {
		// x  y     z		u    v			nx	 ny	  nz
		-1.f, -1.f, -0.6f,	0.f, 0.f,		0.f, 0.f, 0.f,
		0.f, -1.f, 1.f,		0.5f, 0.f,		0.f, 0.f, 0.f,
		1.f, -1.f, -0.6f,		1.f, 0.f,		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,		0.5f, 1.f,		0.f, 0.f, 0.f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1280, 720);
	mainWindow.Initialise();

	camera = Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, 5.f, 0.2f);

	brickTexture = Texture((char*)"Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)"Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(1.f, 1.f, 1.f, 0.1f,
					2.f, -1.f, -2.f, 0.2f);

	shinyMaterial = Material(1.f, 32);
	dullMaterial = Material(0.3f, 4);

	glEnable(GL_DEPTH_TEST);

	CreateObjects();
	CreateShaders();

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0,
		   uniformAmbientColour = 0, uniformAmbientIntensity = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
		   uniformShininess = 0, uniformSpecularIntensity = 0;

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.f);

	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// get & handle input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// clear window
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformModel = shaderList[0].GetModelLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 3.f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}