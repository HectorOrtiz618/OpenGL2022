#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
class Light
{
protected:
	glm::vec3 color;
	GLfloat  ambiantIntensity;
	GLfloat diffuseIntensity;
public:
	Light();
	Light(GLfloat red, GLfloat green,GLfloat blue, GLfloat aIntensity,GLfloat dIntensity);

	virtual void UseLight(GLuint  ambiantIntensityLocation, GLuint  ambiantColorLocation, GLuint  diffuseIntensityLocation);
	~Light();
};

