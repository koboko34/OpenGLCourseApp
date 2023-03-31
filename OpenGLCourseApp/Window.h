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

	bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() const { return glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLuint width, height;
	GLint bufferWidth, bufferHeight;
};

