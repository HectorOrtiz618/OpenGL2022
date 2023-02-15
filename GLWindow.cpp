#include "GLWindow.h"

void GLWindow::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("Pressed %d \n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("Released %d \n", key);
        }
    }
}

void GLWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = (GLfloat)xPos;
        theWindow->lastY = (GLfloat)yPos;
        theWindow->mouseFirstMoved = false;
    }
    theWindow->xChange = xPos - (GLfloat)theWindow->lastX;
    theWindow->yChange = (GLfloat)theWindow->lastY - yPos; //Flipping the subtraction operands or we will get the equivilent of inverted up/down camera controls in video games

    theWindow->lastX = (GLfloat)xPos;
    theWindow->lastY = (GLfloat)yPos;


}

void GLWindow::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, HandleKeys);
    glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

GLWindow::GLWindow()
{
	width = 800;
	height = 600;
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

GLfloat GLWindow::GetXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.f;
    return theChange;
}

GLfloat GLWindow::GetYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.f;
    return theChange;
}

int GLWindow::Initialize()
{
    //init glfw
    if (!glfwInit())
    {
        printf("GLFW Init Fail");
        glfwTerminate();
        return 1;
    }
    //Setup GLFW window propertiers

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.8f);
    //Core profile, no backwards comnpatability, but we do want it to be forward compatable
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    mainWindow = glfwCreateWindow(width, height, "GL Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("Window Creation failed!");
        glfwTerminate();
        return -1;
    }
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    //Set Context for GLEW to use
    glfwMakeContextCurrent(mainWindow);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Handle Key + mouse Inputs
    CreateCallbacks();
    //allow modern extension features

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    //Enable Depth Buffering
    glEnable(GL_DEPTH_TEST);
    //Set viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

GLWindow::~GLWindow()
{
}
