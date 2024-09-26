#include "GLWindow.h"
#include "Error.h"

#include "stb_image.h"

#include <iostream>

namespace
{

void frameBufferSizeCB(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processMouseInput(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = GLWindow::GetInstance().Width() / 2.;
    static float lastY = GLWindow::GetInstance().Height() / 2.;

    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    GLWindow::GetInstance().GetCamera()->ProcessMouseMovement(xoffset, yoffset);
}

void processScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    GLWindow::GetInstance().GetCamera()->ProcessMouseScroll((float)yoffset);
}

auto processKeyboardInput = [](GLFWwindow* window) {
    auto camera = GLWindow::GetInstance().GetCamera();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->Position += camera->MovementSpeed * camera->Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->Position -= camera->MovementSpeed * camera->Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->Position -= glm::normalize(glm::cross(camera->Front, camera->Up)) * camera->MovementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->Position += glm::normalize(glm::cross(camera->Front, GLWindow::GetInstance().GetCamera()->Up)) * GLWindow::GetInstance().GetCamera()->MovementSpeed;
};

}

GLWindow::~GLWindow()
{
    glfwTerminate();
}

GLWindow& GLWindow::GetInstance()
{
    static GLWindow instance;
    return instance;
}

void GLWindow::CreateWindow(int width, int height, const std::string& title)
{
    m_width = width;
    m_height = height;
    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCB);

    stbi_set_flip_vertically_on_load(true);
}

void GLWindow::SetCamera(float x, float y, float z)
{
    m_camera = std::make_shared<Camera>(glm::vec3(x, y, z));
}

std::shared_ptr<Camera> GLWindow::GetCamera()
{
    return m_camera;
}

bool GLWindow::LoadGLLoader() const
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void GLWindow::ProcessMouseInput() const
{
    glfwSetCursorPosCallback(m_window, processMouseInput);
}

void GLWindow::ProcessMouseScroll() const
{
    glfwSetScrollCallback(m_window, processScrollInput);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLWindow::ProcessKeyboardInput() const
{
    processKeyboardInput(m_window);
}

void GLWindow::SwapBuffers()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

int GLWindow::ShouldWindowClose() const
{
    return glfwWindowShouldClose(m_window);
}

void GLWindow::SetBackgroundColor(float r, float g, float b, float a) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(r, g, b, a);
}

void GLWindow::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void GLWindow::EnableBackFaceCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void GLWindow::EnableWireFrame()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int GLWindow::Height() const
{
    return m_height;
}

int GLWindow::Width() const
{
    return m_width;;
}

float GLWindow::AspectRatio() const
{
    return float(m_width) / m_height;;
}

GLFWwindow* GLWindow::Window()
{
    return m_window;
}

void GLWindow::StencilAllowEachFrag()
{
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
    glStencilMask(0xff);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void GLWindow::StencilAllowBorderFrag()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to the stencil buffer
    glDisable(GL_DEPTH_TEST);
}

void GLWindow::SetDefault()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

GLWindow::GLWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
