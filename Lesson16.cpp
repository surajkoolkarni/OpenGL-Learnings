#include "Shader.h"
#include "GLWindow.h"
#include "Model.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};  

int main()
{
    const int width = 1920;
    const int height = 1080;

    std::cout << "Setting up camera and window\n";

    GLWindow::GetInstance().CreateWindow(width, height, "LearnOpenGL");
    GLWindow::GetInstance().SetCamera(1.16f, 1.44f, 4.41f);

    // load opengl function pointers
    if (!GLWindow::GetInstance().LoadGLLoader())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Compiling shaders\n";

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("1.model_loading.vs", "1.model_loading.fs");
    shader->use();

    Model model3D("backpack.obj");

    GLWindow::GetInstance().ProcessMouseInput();
    GLWindow::GetInstance().ProcessMouseScroll();

    std::cout << "Main loop\n";
    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        GLWindow::GetInstance().ProcessKeyboardInput();
        
        GLWindow::GetInstance().SetBackgroundColor(0.05f, 0.05f, 0.05f, 1.0f);
        GLWindow::GetInstance().EnableDepthTest();

        // camera
        glm::mat4 view = GLWindow::GetInstance().GetCamera()->GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(GLWindow::GetInstance().GetCamera()->Zoom), GLWindow::GetInstance().AspectRatio(), 0.1f, 100.0f);

        glm::mat4 model(1.f);

        shader->use();
        shader->setVec3("viewPos", GLWindow::GetInstance().GetCamera()->Position);
        shader->setFloat("shininess", 32.0f);

        shader->setVec3("directionalLight.direction", GLWindow::GetInstance().GetCamera()->Front);
        shader->setVec3("directionalLight.ambient", 0.2, 0.2, 0.2);
        shader->setVec3("directionalLight.diffuse", 0.5, 0.5, 0.5);
        shader->setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        
        for (int i = 0; i < 4; ++i)
        {
            shader->setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);

            shader->setVec3("pointLights[" + to_string(i) + "].ambient", 0.2, 0.2, 0.2);
            shader->setVec3("pointLights[" + to_string(i) + "].diffuse", 0.5, 0.5, 0.5);
            shader->setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

            shader->setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
            shader->setFloat("pointLights[" + to_string(i) + "].linear", 0.09f);
            shader->setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032f);
        }

        shader->setVec3("spotLight.ambient", 0.2, 0.2, 0.2);
        shader->setVec3("spotLight.diffuse", 0.5, 0.5, 0.5);
        shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        
        shader->setVec3("spotLight.direction", GLWindow::GetInstance().GetCamera()->Front);
        shader->setVec3("spotLight.position", GLWindow::GetInstance().GetCamera()->Position);
        shader->setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        shader->setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));

        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        model3D.Draw(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLWindow::GetInstance().SwapBuffers();
    }

    std::cout << "Exit main loop\n";

    model3D.CleanUp();

    // terminate
    GLWindow::GetInstance().Terminate();

    return 0;
}
