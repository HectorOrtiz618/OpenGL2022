#pragma once
#include "PointLight.h"
#include "glm/glm.hpp"
class SpotLight : public PointLight
{
private:
	glm::vec3 direction;
	GLfloat edge, procEdge;
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDir, GLfloat yDir, GLfloat zDir,GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);
	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation,
		GLuint positionLocation,GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);
	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	~SpotLight();
};

