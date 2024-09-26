#include "Shader.h"
#include "GLWindow.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

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

    // SetupDear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    GLWindow::GetInstance().CreateWindow(width, height, "LearnOpenGL");
    GLWindow::GetInstance().SetCamera(1.16f, 1.44f, 4.41f);

    ImGui_ImplGlfw_InitForOpenGL(GLWindow::GetInstance().Window(), true);

    ImGui_ImplOpenGL3_Init("#version 430");

    // load opengl function pointers
    if (!GLWindow::GetInstance().LoadGLLoader())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //GLWindow::GetInstance().SetDefault();

    std::cout << "Compiling shaders\n";

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("1.model_loading.vs", "1.model_loading.fs");
    //std::shared_ptr<Shader> borderShader = std::make_shared<Shader>("1.model_loading.vs", "Outlining.fs");
    // std::shared_ptr<Shader> screenShader = std::make_shared<Shader>("frameBuffer.vs", "frameBuffer.fs");
    // screenShader->use();
    // screenShader->setInt("screenTexture", 0);

    std::unique_ptr<Model> model3D = std::make_unique<Model>("backpack.obj");

    //GLWindow::GetInstance().ProcessMouseInput();
    //GLWindow::GetInstance().ProcessMouseScroll();

    std::cout << "Main loop\n";

    glm::vec2 translationVec(0.f, 0.f);
    float zoom = tan(glm::radians(70.f));
    glm::vec3 rotationAxis(0.f, 1.f, 0.f);
    float angle = 0.f;

    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Manipulate the model");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat2("Pan", &translationVec[0], -5.0f, 5.f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("Zoom fov", &zoom, 0.f, 500.f);
        ImGui::InputFloat3("axis of rotation", &rotationAxis[0]);
        ImGui::SliderAngle("angle of rotation", &angle);
        ImGui::End();

        GLWindow::GetInstance().ProcessKeyboardInput();
        
        GLWindow::GetInstance().SetBackgroundColor(0.5f, 0.5f, 0.5f, 1.0f);
        GLWindow::GetInstance().EnableDepthTest();

        // camera
        glm::mat4 view = GLWindow::GetInstance().GetCamera()->GetViewMatrix();

        glm::mat4 projection = glm::perspective(glm::radians(zoom), GLWindow::GetInstance().AspectRatio(), 0.1f, 100.0f);

        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3{ translationVec, 0.f });
        model = glm::rotate(model, angle, rotationAxis);

        glm::mat4 anotherModel = glm::translate(glm::mat4(1.f), glm::vec3(5.f, 5.f, 0.f));
        
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

        //GLWindow::GetInstance().EnableBackFaceCulling();

        //GLWindow::GetInstance().StencilAllowEachFrag();

        //model3D.Draw(shader);

        // screenShader->use();

        ImGui::Render();

        shader->use();
        model3D->Draw(shader);

        shader->setMat4("model", anotherModel);
        shader->use();
        model3D->Draw(shader);

        //glm::mat4 modelBorder(1.0);
        //modelBorder = glm::scale(modelBorder, glm::vec3(1.05, 1.05, 1.05));

        //borderShader->use();
        //borderShader->setMat4("model", modelBorder);
        //borderShader->setMat4("view", view);
        //borderShader->setMat4("projection", projection);
        //
        //GLWindow::GetInstance().StencilAllowBorderFrag();

        // now draw scaled up object but only if stencil buffer is not 1 (is 0)
        //model3D.Draw(borderShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLWindow::GetInstance().SwapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << "Exit main loop\n";

    return 0;
}
