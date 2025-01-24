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
#include "ShaderManager.h"
#include "Shape.h"
#include "Sphere.h"
#include "Extras.h" //all my helper shit that doesnt really fit in anywhere

//program params
unsigned int WIDTH = 1280, HEIGHT = 720;
constexpr bool USE_VSYNC = false;
constexpr float YAW = 0.022f, PITCH = 0.022f; //same turn speed as CS2, UE5 default = 0.07

//functions
void MousePosCallBack(GLFWwindow* window, double xpos, double ypos);
void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyPressed();

//program variables
MouseInputMode CurrentMode = WINDOW_MODE;

float delta = 0.0f;
float RenderDelta = 0.0f;

std::vector<Shape*> Objects;

GLFWwindow* window;

Camera cam;

glm::mat4 view = cam.GetViewMatrix();
glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

float lastX = 400.0f;
float lastY = 300.0f;
float sensitivity = 0.1f;
bool firstMouse = true;

int main(){
    
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

    //glfw callbacks
    glfwSetMouseButtonCallback(window, MouseCallBack);
    glfwSetCursorPosCallback(window, MousePosCallBack);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        glViewport(0, 0, width, height);
        HEIGHT = height;
        WIDTH = width;
        std::cout << "Viewport resized to " << width << "x" << height << std::endl;});

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(USE_VSYNC); //vsyncs

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    ShaderManger shaderManager;

    Sphere light = Sphere(1, 36, ShaderProgram::UnlitShader);
    Objects.push_back(&light);

    Sphere sphere = Sphere(5, 48);
    sphere.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.0, -5.0, -50.0)));
    sphere.SetColor(glm::vec4(1, 0, 0, 1));
    Objects.push_back(&sphere);

    Sphere sphere2 = Sphere(5, 48);
    sphere2.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.0, -5.0, -25.0)));
    sphere2.SetColor(glm::vec4(0, 1, 0, 1));
    Objects.push_back(&sphere2);
  
    Sphere sphere3 = Sphere(5, 48);
    sphere3.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(-0.0, -5.0, 0.0)));
    sphere3.SetColor(glm::vec4(0, 0, 1, 1));
    Objects.push_back(&sphere3);

    //skybox
    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO(108 * sizeof(float), skyboxVertices);
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

        static glm::vec3 light(10, 10, 0);
        static glm::vec3 lightCol(1, 1, 1);

        static float ambient = 0.15, spec = 0.5;

        Objects[0]->SetColor(glm::vec4(lightCol, 1.0));
        Objects[0]->SetTransform(glm::translate(glm::mat4(1.0), light));

        for (Shape* s : Objects) {  //render all objects stored in list
            s->RenderStart();
            Shader* currentshader = shaderManager.GetShader(s->GetShader());
            currentshader->Bind();
            currentshader->SetUniformMat4f("u_view", view);
            currentshader->SetUniformMat4f("u_proj", proj);
            currentshader->SetUniformMat4f("u_model", s->GetModelMatrix());
            currentshader->SetUniform3f("u_sphereColor", s->GetColor());
            currentshader->SetUniform1f("u_ambientStrength", ambient);  // s->GetAmbient
            currentshader->SetUniform1f("u_specularStrength", spec);    // s->GetSpecular
            currentshader->SetUniform3f("u_lightPos", light);
            currentshader->SetUniform3f("u_viewPos", cam.GetPos());
            currentshader->SetUniform3f("u_lightColor", lightCol);
            s->RenderStop();
            currentshader->UnBind();
        }

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
        ImGui::Text("Cameramode Mode: %d", CurrentMode);

        ImGui::ColorEdit3("Light Color: ", (float*) &lightCol);
        ImGui::NewLine();
        ImGui::DragFloat3("Light position", (float*)&light);

        ImGui::DragFloat("Ambient Strength: ", &ambient, 0.001f, 0.0f, 1.0f);
        ImGui::DragFloat("Specular Strength: ", &spec, 0.001f, 0.0f, 1.0f);

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

    //camera movement
    if (CurrentMode == MouseInputMode::CAMERA_MODE){
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
    }   else {

    }

}

void MouseCallBack(GLFWwindow* window, int button, int action, int mods) {

    if (ImGui::GetIO().WantCaptureMouse) { //ignore mouseclicks on imgui
        return;
    }

    if (CurrentMode == WINDOW_MODE){
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            ToggleMouseInputMode(window, CurrentMode);
            firstMouse = true;
        }
    } else { //cameramode

    }
}

void MousePosCallBack(GLFWwindow* window, double xpos, double ypos) {
    if (CurrentMode == WINDOW_MODE)
        return;
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false; 
    }

    float xOffset = (xpos - lastX) * YAW;
    float yOffset = (lastY - ypos) * PITCH;

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouse(xOffset, yOffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (CurrentMode == WINDOW_MODE){    //close
                break; //comment out to close on esc
                std::cout << "Escape key pressed, closing window." << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            } else {    //leave cameramode
                ToggleMouseInputMode(window, CurrentMode);
            }
            break;
        }
    }
}
