#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambiantIntensity = 1.f;
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir,GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambiantIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambiantColorLocation, color.x, color.y, color.z);
	glUniform1f(ambiantIntensityLocation,ambiantIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{

}
