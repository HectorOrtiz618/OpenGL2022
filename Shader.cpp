#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

    pointLightCount = 0;
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
