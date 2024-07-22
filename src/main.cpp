#include <iostream>
#include <chrono>

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"

float* CreateSphere(const float radius, const int PointAmount);
unsigned int* CreateSphereIndices(const int PointAmount);

constexpr int WIDTH = 1280, HEIGHT = 720;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 50.0), // pos
                             glm::vec3(0.0, 0.0, 0.0),  // look
                             glm::vec3(0.0, 1.0, 0.0)); // up

float *vertices = CreateSphere(5, 48);

unsigned int* indices = CreateSphereIndices(48);

int main(void)
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
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

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray VAO;
    VertexBuffer VBO(3 * 48 * 48 * sizeof(float), vertices);
    ElementArrayBuffer EBO(6 * 47 * 47 * sizeof(unsigned int), indices);

    VAO.Bind();
    VAO.Bind();
    EBO.Bind();
    VAO.BindVertexBuffer(VBO, 0, 3, GL_FLOAT, GL_FALSE , 3 * sizeof(float), (void*)0);
    VAO.BindElementArrayBuffer(EBO);

    Shader shader("res/shaders/BasicShader.shader");
    shader.Bind();
    shader.SetUniformMat4f("u_view", view);
    shader.SetUniformMat4f("u_proj", proj);
    shader.SetUniformMat4f("u_model", model);
    shader.Bind();

    auto now = std::chrono::system_clock::now();
    auto last = std::chrono::system_clock::now();
    float delta = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        last = now;
        now = std::chrono::system_clock::now();
        delta = (float) (now - last).count() * 0.0000001;
        static float color[4] = {0.1, 0.1, 0.1, 1.0};
        glClearColor(color[0], color[1], color[2], color[3]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::ColorEdit4("clear_Color", color);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        shader.SetUniformMat4f("u_view", view);
        shader.SetUniformMat4f("u_proj", proj);
        shader.SetUniformMat4f("u_model", model);

        //RENDER SPHERE
        glDrawElements(GL_TRIANGLES, 6 * (48-1) * (48 - 1), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


constexpr float PI = 3.1415926535897932;

float* CreateSphere(const float radius, const int PointAmount) {
    const int totalelements = 3 * PointAmount * PointAmount; // Adjusted for more points

    float* points = new float[totalelements];

    int index = 0;
    for (size_t i = 0; i < PointAmount; i++) {
        float phi = PI * i / (PointAmount - 1); // Ranges from 0 to PI

        for (size_t j = 0; j < PointAmount; j++) {
            float theta = 2 * PI * j / (PointAmount - 1); // Ranges from 0 to 2*PI

            points[index++] = radius * sin(phi) * cos(theta);
            points[index++] = radius * sin(phi) * sin(theta);
            points[index++] = radius * cos(phi);
        }
    }
    return points;
}

unsigned int* CreateSphereIndices(const int PointAmount) {
    const int totalIndices = 6 * (PointAmount - 1) * (PointAmount - 1);
    unsigned int* indices = new unsigned int[totalIndices];

    int index = 0;
    for (int i = 0; i < PointAmount - 1; i++) {
        for (int j = 0; j < PointAmount - 1; j++) {
            int current = i * PointAmount + j;
            int next = current + PointAmount;

            indices[index++] = current;
            indices[index++] = next;
            indices[index++] = current + 1;

            indices[index++] = current + 1;
            indices[index++] = next;
            indices[index++] = next + 1;
        }
    }
    return indices;
}