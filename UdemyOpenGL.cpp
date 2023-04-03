// UdemyOpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "GL/glew.h"
#include "glfw3.h"
#include "GLWindow.h"
#include "Camera.h"
#include "glm/glm.hpp";
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "assimp/Importer.hpp"
#include "Model.h"

//Vertex Shader
GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0,uniformDirectionalLightTransform = 0,uniformOmniLightPos = 0,uniformFarPlane = 0;
std::vector<Mesh*> meshList;
Camera camera;
glm::mat4 projection;
std::vector<Shader>shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
bool bDirection = true;
float triOffset = 0.f;
float triMaxOffset = 0.7f;
float triInfrement = 0.005f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
Texture dirtTexture("textures/dirt.png");  
Texture brickTexture("textures/brick.png");
Texture plainTexture("textures/plain.png");
DirectionalLight mainLight;
PointLight pointLight[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;
SpotLight spotLight[MAX_SPOT_LIGHTS];
unsigned int spotLightCount = 0;
Material shinyMaterial;
Material dullMaterial;
Material plainMaterial;
Model xWing;
static const char* vShader = "shaders/shader.vert";

static const char* fShader = "shaders/shader.frag";
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }
    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y, vertices[nOffset + 2] = vec.z;
    }
}
void CreateObjects()
{
    unsigned int indices[] =
    {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };
    GLfloat vertices[] =
    {
        //x     y     z     u     v         nx  ny  nz
        -1.0f, -1.0f, 0.f,  0.f,  0.f,      0.0f,0.0f,0.0f,
        0.f, -1.0f, 1.0f,   0.5f,  0.f,     0.0f,0.0f,0.0f,
        1.0f,-1.0f, 0.0f,   1.f,   0.f,     0.0f,0.0f,0.0f,
        0.0f, 1.0f, 0.0f,   0.5f,   1.f,    0.0f,0.0f,0.0f
    };

    unsigned int floorIndices[] = {
    0, 2, 1,
    1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
    };
    calcAverageNormals(indices, 12, vertices, 32, 8, 5);
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);

    //printf("obj1 loaded into meshList\n");
}
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader, fShader);
    
    shaderList.push_back(*shader1);
    //directionalShadowShader = new Shader();
    directionalShadowShader.CreateFromFile("shaders/directional_shadow_map.vert", "shaders/directional_shadow_map.frag");
    omniShadowShader.CreateFromFile("shaders/omni_shadow_map.vert", "shaders/omni_shadow_map.frag","shaders/omni_shadow_map.geo",true);
}
void RenderScene()
{
    glm::mat4 model(1.0f);


    model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); 

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();

    //clearing transforms for new model
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 4.f, -5.0f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.f, 1.0f, 5.0f));
    model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xWing.RenderModel();
}
void DirectionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.GetModelLocation();
    glm::mat4 lightTransform = mainLight.CalculateLightTransform();
    directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

    directionalShadowShader.ValidateProgram();
    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void OmniShadowMapPass(PointLight* light)
{

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    
    omniShadowShader.UseShader();
    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);


    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetLightMatrices(light->CalculateLightTransform());
    omniShadowShader.ValidateProgram();

    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    shaderList[0].UseShader();

    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();
    
    uniformEyePosition = shaderList[0].GetEyePosition();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensity();
    uniformShininess = shaderList[0].GetShininessLocation();

    glViewport(0, 0, 1366, 768);

    //clear window
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    
    
    shaderList[0].SetDirectionalLight(&mainLight);
    shaderList[0].SetPointLight(pointLight, pointLightCount,3,0);
    shaderList[0].SetSpotLights(spotLight, spotLightCount,3+pointLightCount,pointLightCount);
    glm::mat4 lightTransform= mainLight.CalculateLightTransform();
    shaderList[0].SetDirectionalLightTransform(&lightTransform);

    mainLight.GetShadowMap()->Read(GL_TEXTURE2);
    shaderList[0].SetTexture(1);
    shaderList[0].SetDirectionalShadowMap(2);

    glm::vec3 lowerLight = camera.GetCameraPosition();
    lowerLight.y -= 0.3f;

    spotLight[0].SetFlash(lowerLight, camera.GetCameraDirection());
    shaderList[0].ValidateProgram();
    RenderScene();


    //glm matrices MUIST be initilized as glm::mat4 model(1.0f); OR YOU WILL GET GARBAGE TRANSFORM DATA
   //DirectionalShadowMapPass(&mainLight);

}
int main()
{
    GLWindow mainWindow = GLWindow(1366, 768);
    mainWindow.Initialize();
    
    CreateObjects();
    CreateShaders();
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f,0.f, 5.0f, 0.5f);
    projection = glm::perspective(glm::radians(45.0f), mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

  
    brickTexture.LoadTextureWithAlpha();
    dirtTexture.LoadTextureWithAlpha();
    plainTexture.LoadTextureWithAlpha();

    shinyMaterial = Material(1.0f,32);
    dullMaterial = Material(0.3f, 4);

    xWing = Model();
    xWing.LoadModel("models/x-wing.obj");
    

    mainLight = DirectionalLight
    (4096.f, 4096.f,
        1.0f, 1.0f, 1.0f,
        0.1f, 
        0.0f,
        -15.0f, -10.0f, 
        0.3f);
    pointLight[0] = PointLight(2048.f,2048.f,0.01f,200.f,0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.f, 0.0f, 0.3f, 0.2f, 0.1f);
    pointLightCount++;
    spotLight[0] = SpotLight(2048.f,2048.f,1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,1.0f,0.0f,0.f,20.f);
    spotLightCount++;
    spotLight[1] = SpotLight(2048.f, 2048.f,1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.5f, 0.0f, -100.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.f);
    spotLightCount++;
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        glfwPollEvents();
        camera.keyControls(mainWindow.GetKeys(),deltaTime);
        camera.mouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());
        
        if (mainWindow.GetKeys()[GLFW_KEY_F])
        {
            spotLight[0].Toggle();
            mainWindow.GetKeys()[GLFW_KEY_F] = false;
        }
        DirectionalShadowMapPass(&mainLight);
        for (size_t i = 0; i < pointLightCount; i++)
        {
            OmniShadowMapPass(&pointLight[i]);
        }
        for (size_t i = 0; i < spotLightCount; i++)
        {
            OmniShadowMapPass(&spotLight[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();

    }
    return 0;
}
