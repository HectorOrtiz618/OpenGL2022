#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}
void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFromFile(vertexLocation);
    std::string fragmentString = ReadFromFile(fragmentLocation);
    CompileShader(vertexString.c_str(), fragmentString.c_str());
}
std::string Shader::ReadFromFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);
    if (!fileStream.is_open())
    {
        printf("Failed to load %s! File does not exist!", fileLocation);
        return"";
    }
    std::string line = "";


    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line+"\n");
        //line = "";
    }

    fileStream.close();
    return content;
}
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();
    if (!shaderID)
    {
        printf("Compiling Shader Program Failed\n");
        return;
    }
    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Linking Program: %s", &eLog);
    }
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Validating Program: %s", &eLog);
    }
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = (glGetUniformLocation(shaderID, "view"));
    uniformDirectionalLight.uniformAmbiantColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbiantIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambiantIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.color", i);
        uniformPointLight[i].uniformAmbiantColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.ambiantIntensity", i);
        uniformPointLight[i].uniformAmbiantIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLight[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.base.color", i);
        uniformSpotLight[i].uniformAmbiantColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.base.ambiantIntensity", i);
        uniformSpotLight[i].uniformAmbiantIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLight[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
    uniformTexture = glGetUniformLocation(shaderID, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

}
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);


    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: %s", shaderType, &eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}
GLuint Shader::GetProjectionLocation() const
{
    return uniformProjection;
}
GLuint Shader::GetModelLocation() const
{
    return uniformModel;
}
GLuint Shader::GetViewLocation() const
{
    return uniformView;
}
GLuint Shader::GetAmbiantIntensity() const
{
    return uniformDirectionalLight.uniformAmbiantIntensity;
}
GLuint Shader::GetAmbiantColor() const
{
    return uniformDirectionalLight.uniformAmbiantColor;
}
GLuint Shader::GetDiffuseIntensity() const
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation() const
{
    return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetEyePosition() const
{
    return uniformEyePosition;
}
GLuint Shader::GetSpecularIntensity() const
{
    return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation() const
{
    return uniformShininess;
}
void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbiantIntensity, uniformDirectionalLight.uniformAmbiantColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}
void Shader::SetPointLight(PointLight* pLight, unsigned int pCount)
{
    if (pCount > MAX_POINT_LIGHTS)
    {
        pCount = MAX_POINT_LIGHTS;
    }
    glUniform1i(uniformPointLightCount, pCount);
    for (int i = 0; i < pCount; i++)
    {
        pLight[i].UseLight(uniformPointLight[i].uniformAmbiantIntensity, uniformPointLight[i].uniformAmbiantColor, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}
void Shader::SetSpotLights(SpotLight* sLight, unsigned int sCount)
{
    if (sCount > MAX_POINT_LIGHTS)
    {
        sCount = MAX_POINT_LIGHTS;
    }
    glUniform1i(uniformSpotLightCount, sCount);
    for (int i = 0; i < sCount; i++)
    {
        sLight[i].UseLight(uniformSpotLight[i].uniformAmbiantIntensity, uniformSpotLight[i].uniformAmbiantColor, uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
    }
}
void Shader::SetTexture(GLuint textureUnit)
{
    glUniform1i(uniformTexture, textureUnit);
}
void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}
void Shader::SetDirectionalLightTransform(glm::mat4 *lTransform)
{
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}
void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
         glDeleteProgram(shaderID);
         shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    ClearShader();
}
