#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
class Light
{
private:
	glm::vec3 color;
	GLfloat  ambiantIntensity;
	glm::vec3 direction;
	GLfloat diffuseIntensity;
public:
	Light();
	Light(GLfloat red, GLfloat green,GLfloat blue, GLfloat aIntensity,GLfloat xDir,GLfloat yDir,GLfloat zDir,GLfloat dIntensity);

	void UseLight(GLuint  ambiantIntensityLocation, GLuint  ambiantColorLocation, GLuint  diffuseIntensityLocation, GLuint  directionLocation);
	~Light();
};

