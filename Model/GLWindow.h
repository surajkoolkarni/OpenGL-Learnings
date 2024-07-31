#pragma once

#include "DllExport.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

#include <functional>
#include <memory>
#include <string>

namespace
{

void frameBufferSizeCB(GLFWwindow* window, int width, int height);

}

class MODEL_API GLWindow
{
public:
    ~GLWindow();

    static GLWindow& GetInstance();

    void CreateWindow(int width, int height, const std::string& title);

    void SetCamera(float x, float y, float z);

    std::shared_ptr<Camera> GetCamera();

    bool LoadGLLoader() const;

    void ProcessKeyboardInput() const;

    void ProcessMouseScroll() const;

    void ProcessMouseInput() const;

    void SwapBuffers();

    int ShouldWindowClose() const;

    void SetBackgroundColor(float r, float g, float b, float a) const;

    void EnableDepthTest();

    void EnableBackFaceCulling();

    void EnableWireFrame();

    void StencilAllowEachFrag();

    void StencilAllowBorderFrag();

    void SetDefault();

    int Height() const;

    int Width() const;

    float AspectRatio() const;

private:
    GLWindow();

private:
    GLFWwindow* m_window;
    std::shared_ptr<Camera> m_camera;

    int m_width, m_height;
};
