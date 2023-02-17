#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
private:
	glm::vec3 direction;
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	virtual void UseLight(GLuint  ambiantIntensityLocation, GLuint  ambiantColorLocation, GLuint  diffuseIntensityLocation, GLuint  directionLocation);
	~DirectionalLight();
};

