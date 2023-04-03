#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:

	Window();
	Window(GLuint windowWidth, GLuint windowHeight);
	~Window();

	int Initialise();

	GLfloat GetBufferWidth() const { return bufferWidth; }
	GLfloat GetBufferHeight() const { return bufferHeight; }

	bool* GetKeys() { return keys; }
	GLfloat GetXChange();
	GLfloat GetYChange();

	bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() const { return glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLuint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMove;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

