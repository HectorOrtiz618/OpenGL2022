#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
private:
	glm::vec3 position, front, up, right,worldup;
	GLfloat yaw, pitch,movementSpeed,turnSpeed;
	void update();
	
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);
	~Camera();
	void keyControls(bool* keys,GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 calculateViewMatrix();
	glm::vec3 GetCameraPosition()const;
	glm::vec3 GetCameraDirection()const;
};

