#include <iostream>
#include <chrono>

#include <filesystem>
#include <stb_image/stb_image.h>

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
#include "Camera.h"

GLuint loadCubemap(std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

float skyboxVertices[];
std::vector<std::string> faces{
    "res/textures/skybox/right.jpg",
    "res/textures/skybox/left.jpg",
    "res/textures/skybox/top.jpg",
    "res/textures/skybox/bottom.jpg",
    "res/textures/skybox/front.jpg",
    "res/textures/skybox/back.jpg"
};

float* CreateSphere(const float radius, const int PointAmount);
unsigned int* CreateSphereIndices(const int PointAmount);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyPressed();

constexpr int WIDTH = 1280, HEIGHT = 720;
float delta = 0.0f;
float RenderDelta = 0.0f;

GLFWwindow* window;

Camera cam;

glm::mat4 view = cam.GetViewMatrix();
glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
glm::mat4 model = glm::mat4(1.0f);

float* vertices = CreateSphere(5, 48);
unsigned int* indices = CreateSphereIndices(48);

int main(void){

    if (!glfwInit()) {
        std::cout << "error initializing glfw" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "My Open GL Program", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cout << "error creating window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(false); //vsyncs

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    VertexArray VAO;
    VertexBuffer VBO(3 * 48 * 48 * sizeof(float), vertices);
    ElementArrayBuffer EBO(6 * 47 * 47 * sizeof(unsigned int), indices);

    VAO.Bind();
    VAO.Bind();
    EBO.Bind();
    VAO.AddVertexBuffer(VBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    VAO.BindElementArrayBuffer(EBO);

    Shader shader("res/shaders/BasicShader.shader");
    shader.Bind();
    shader.SetUniformMat4f("u_view", view);
    shader.SetUniformMat4f("u_proj", proj);
    shader.SetUniformMat4f("u_model", model);
    shader.UnBind();

    VAO.Unbind();
    VBO.Unbind();
    
    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO( 108 * sizeof(float), skyboxVertices);
    skyboxVAO.Bind();
    skyboxVAO.AddVertexBuffer(skyboxVBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    skyboxVAO.Unbind();
    skyboxVBO.Unbind();
    
    Shader skyboxShader("res/shaders/Skybox.shader");
    skyboxShader.Bind();
    skyboxShader.UnBind();

    skyboxVAO.Unbind();
    skyboxVBO.Unbind();

    GLuint cubemapTexture = loadCubemap(faces);    

    auto now = std::chrono::system_clock::now();
    auto last = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(window)) { // window/game loop
        //update values
        last = now;
        now = std::chrono::system_clock::now();
        delta = (float)(now - last).count() / 10000;
        RenderDelta = 1000.0f / ImGui::GetIO().Framerate;
        view = cam.GetViewMatrix();

        keyPressed(); //keypress check

        /* Render here */
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //RENDER SPHERE
        VAO.Bind();
        shader.Bind();
        shader.SetUniformMat4f("u_view", view);
        shader.SetUniformMat4f("u_proj", proj);
        shader.SetUniformMat4f("u_model", model);
        glDrawElements(GL_TRIANGLES, 6 * (48 - 1) * (48 - 1), GL_UNSIGNED_INT, 0);
        shader.UnBind();
        VAO.Unbind();

        // skybox here
        glDepthFunc(GL_LEQUAL);
        skyboxVAO.Bind();
        skyboxShader.Bind();
        skyboxShader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(view)));
        skyboxShader.SetUniformMat4f("u_proj", proj);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxShader.UnBind();
        skyboxVAO.Unbind();

        //imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Position: x:%.3f  y:%.3f  z:%.3f ", cam.m_Position.x, cam.m_Position.y, cam.m_Position.z);
        ImGui::Text("Look Dir: x:%.3f y:%.3f z:%.3f", cam.m_Front.x, cam.m_Front.y, cam.m_Front.z);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void keyPressed() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
        cam.ProcessKeyboard(LEFT, delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        cam.ProcessKeyboard(RIGHT, delta);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.ProcessKeyboard(UP, delta);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        cam.ProcessKeyboard(DOWN, delta);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam.ProcessKeyboard(DOWN, delta);

    //rotation temp -> TODO use mouse xy
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam.ProcessKeyboard(rotLEFT, delta);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam.ProcessKeyboard(rotRIGHT, delta);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam.ProcessKeyboard(rotUP, delta);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam.ProcessKeyboard(rotDOWN, delta);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            std::cout << "Escape key pressed, closing window." << std::endl;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
    }
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

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
