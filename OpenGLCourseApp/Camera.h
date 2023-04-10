#pragma once

#include <GL/glew.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 CalculateViewMatrix();

	glm::vec3 GetCameraPosition() const { return position; }

private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void Update();


};

