#pragma once
#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>
class DirectionalLight : public Light
{
private:
	glm::vec3 direction;
public:
	DirectionalLight();

	DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	glm::mat4 CalculateLightTransform();
	virtual void UseLight(GLuint  ambiantIntensityLocation, GLuint  ambiantColorLocation, GLuint  diffuseIntensityLocation, GLuint  directionLocation);
	~DirectionalLight();
};

