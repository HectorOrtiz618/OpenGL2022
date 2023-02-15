#pragma once
#include<stdio.h>
#include "GL/glew.h"
#include "glfw3.h"
class GLWindow
{
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];
	GLfloat lastX, lastY;
	GLfloat xChange, yChange;
	bool mouseFirstMoved;
	//GLFW needs a static function to call back to for input
	static void HandleKeys(GLFWwindow *window,int key,int code,int action,int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
	void CreateCallbacks();
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	GLfloat GetBufferWidth()const { return bufferWidth; }
	GLfloat GetBufferHeight()const { return bufferHeight; }
	bool GetShouldClose()const { return glfwWindowShouldClose(mainWindow); }
	bool* GetKeys() { return keys; }
	GLfloat GetXChange();
	GLfloat GetYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	int Initialize();
	~GLWindow();
};

