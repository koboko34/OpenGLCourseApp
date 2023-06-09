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

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "Skybox.h"

const float toRadians = 3.14159265f / 180.f;

GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0,
	uniformShininess = 0, uniformSpecularIntensity = 0,
	uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Window mainWindow;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture floorTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

Skybox skybox;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;

GLfloat blackhawkAngle = 0.f;

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
		1.f, -1.f, -0.6f,	1.f, 0.f,		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,		0.5f, 1.f,		0.f, 0.f, 0.f
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	unsigned int floorIndices[] = {
	0, 2, 1,
	1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* floorMesh = new Mesh();
	floorMesh->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floorMesh);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
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

	// floor
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, -2.f, 0.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-15.f, -1.0f, 5.f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.RenderModel();

	blackhawkAngle += 0.06f;
	if (blackhawkAngle > 360.f)
	{
		blackhawkAngle -= 360.f;
	}

	model = glm::mat4(1.f);
	model = glm::rotate(model, blackhawkAngle * toRadians, glm::vec3(0.f, 1.f, 0.f));
	model = glm::translate(model, glm::vec3(10.f, 3.0f, 0.f));
	model = glm::rotate(model, 20.f * toRadians, glm::vec3(0.f, 0.f, 1.f));
	model = glm::rotate(model, -90.f * toRadians, glm::vec3(1.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HandleOmniShadows()
{
	for (size_t i = 0; i < pointLightCount; i++)
	{
		OmniShadowMapPass(&pointLights[i]);
	}

	for (size_t i = 0; i < spotLightCount; i++)
	{
		OmniShadowMapPass(&spotLights[i]);
	}
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1920, 1080);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);
	
	shaderList[0].UseShader();

	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformShininess = shaderList[0].GetShininessLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lightTransform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLightPos = camera.GetCameraPosition();
	lowerLightPos.y -= 0.3f;
	spotLights[0].SetFlash(lowerLightPos, camera.GetCameraDirection());

	shaderList[0].Validate();

	RenderScene();
}

int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	camera = Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, 12.f, 0.2f);

	brickTexture = Texture((char*)"Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture((char*)"Textures/dirt.png");
	dirtTexture.LoadTextureA();
	floorTexture = Texture((char*)"Textures/plain.png");
	floorTexture.LoadTextureA();

	mainLight = DirectionalLight(4096, 4096,
								1.f, 0.5f, 0.1f,
								0.3f, 0.8f,
								-10.f, -12.f, 21.f);

	pointLights[0] = PointLight(1024, 1024,
								0.01f, 100.f,
								0.f, 0.f, 1.f,
								0.0f, 0.f,
								4.f, 2.f, 0.f,
								0.3f, 0.02f, 0.01f);
	//pointLightCount++;

	pointLights[1] = PointLight(1024, 1024,
								0.01f, 100.f,
								0.f, 1.f, 0.f,
								0.f, 0.f,
								-4.f, 2.f, 0.f,
								0.3f, 0.02f, 0.01f);
	//pointLightCount++;

	spotLights[0] = SpotLight(1024, 1024,
								0.01f, 100.f,
								1.f, 1.f, 1.f,
								0.1, 2.f,
								0.f, -1.5f, 0.f,
								-100.f, -1.f, 0.f,
								1.f, 0.02f, 0.01f,
								20.f);
	//spotLightCount++;

	
	spotLights[1] = SpotLight(1024, 1024,
								0.01f, 100.f,
								1.f, 1.f, 1.f,
								0.1, 1.f,
								0.f, -1.5f, 0.f,
								-100.f, -1.f, 0.f,
								1.f, 0.f, 0.f,
								20.f);
	//spotLightCount++;

	/*
	spotLights[2] = SpotLight(1024, 1024,
								0.01f, 100.f,
								1.f, 1.f, 1.f,
								0.1, 2.f,
						  		0.f, -1.5f, 0.f,
								-100.f, -1.f, 0.f,
								1.f, 0.02f, 0.01f,
								20.f);
	spotLightCount++;
	*/

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	shinyMaterial = Material(4.f, 128);
	dullMaterial = Material(0.3f, 4);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");
	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj");

	CreateObjects();
	CreateShaders();

	glm::mat4 projection = glm::perspective(glm::radians(60.f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.f);

	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// get & handle input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		if (mainWindow.GetKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			mainWindow.GetKeys()[GLFW_KEY_L] = false;
		}

		DirectionalShadowMapPass(&mainLight);
		HandleOmniShadows();
		RenderPass(camera.CalculateViewMatrix(), projection);

		// glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}