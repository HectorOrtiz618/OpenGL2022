#include "Camera.h"

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position+front, up);
}

glm::vec3 Camera::GetCameraPosition() const
{
	return position;
}

Camera::Camera()
{

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldup = startUp;
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = startMovementSpeed;
	turnSpeed = startTurnSpeed;
}

Camera::~Camera()
{
}

void Camera::keyControls(bool* keys,GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		position += front * (movementSpeed * deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * (movementSpeed * deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * (movementSpeed * deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * (movementSpeed * deltaTime);
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	yaw += xChange;
	pitch += yChange;

	if (pitch >= 89.0f)
	{
		pitch = 89.0;
	}
	if (pitch <= -89.0f)
	{
		pitch = -89.0;
	}
	update();
}
