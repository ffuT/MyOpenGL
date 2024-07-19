#include <iostream>
#include <chrono>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

constexpr int WIDTH = 1280, HEIGHT = 720;

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "MY OPENGL WORLD WOW!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(true); //vsync

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    auto now = std::chrono::system_clock::now();
    auto last = std::chrono::system_clock::now();
    auto second = now + std::chrono::seconds(1);
    float delta = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        last = now;
        now = std::chrono::system_clock::now();
        delta = (float)(now - last).count() / 10000000;
        static float color[4] = {0.1, 0.1, 0.1, 1.0};
        glClearColor(color[0], color[1], color[2], color[3]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::ColorEdit4("clear_Color", color);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}