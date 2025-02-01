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

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Sphere.h"
#include "Extras.h" //all my helper shit that doesnt really fit in anywhere

//program params
unsigned int WIDTH = 1280, HEIGHT = 720;
constexpr float YAW = 0.022f, PITCH = 0.022f; //same turn speed as CS2, UE5 default = 0.07
constexpr bool USE_VSYNC = false;

//functions
void MousePosCallBack(GLFWwindow* window, double xpos, double ypos);
void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyPressed(float delta);
void toggleFullscreen();

//program variables
bool isFullscreen = false;
int windowedX, windowedY, windowedWidth, windowedHeight; // save windowed mode state
float delta = 0.0f;

GLFWwindow* window;

std::vector<Shape*> Objects;

MouseInputMode CurrentWindowMode = WINDOW_MODE;

Camera cam;

glm::mat4 proj = glm::perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

float lastX = 400.0f;
float lastY = 300.0f;
float sensitivity = 0.1f;
bool firstMouse = true;

int main(){
    if (!glfwInit()) {
        std::cout << "error initializing glfw" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "Loading...", NULL, NULL);
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

    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSwapInterval(USE_VSYNC); //vsyncs

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black color
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window); // Clear to black while initializing

    //glfw callbacks
    glfwSetMouseButtonCallback(window, MouseCallBack);
    glfwSetCursorPosCallback(window, MousePosCallBack);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        glViewport(0, 0, width, height);
        HEIGHT = height;
        WIDTH = width;});

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    Renderer renderer = Renderer();

    Sphere light = Sphere(1, 16, ShaderProgram::UnlitShader);
    Objects.push_back(&light);

    Sphere sphere = Sphere(5, 32);
    sphere.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(-25.0, -5.0, -50.0)));
    sphere.SetColor(glm::vec4(1, 0, 0, 1));
    Objects.push_back(&sphere);

    Sphere sphere2 = Sphere(5, 32);
    sphere2.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -5.0, -50.0)));
    sphere2.SetColor(glm::vec4(0, 1, 0, 1));
    Objects.push_back(&sphere2);
  
    Sphere sphere3 = Sphere(5, 32);
    sphere3.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(25.0, -5.0, -50.0)));
    sphere3.SetColor(glm::vec4(0, 0, 1, 1));
    Objects.push_back(&sphere3);
    
    //skybox
    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO(108 * sizeof(float), skyboxVertices);
    skyboxVAO.Bind();
    skyboxVAO.AddVertexBuffer(skyboxVBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    Shader skyboxShader("res/shaders/Skybox.shader");
    skyboxVAO.Unbind();
    skyboxVBO.Unbind();
    GLuint cubemapTexture = loadCubemap(skyboxes::Space);

    auto now = std::chrono::system_clock::now();
    auto last = std::chrono::system_clock::now();
    glfwSetWindowTitle(window, "My OpenGL Program");
    while (!glfwWindowShouldClose(window)) { // window/game loop
        //update values
        last = now;
        now = std::chrono::system_clock::now();
        delta = (float)(now - last).count() / 10000;

        keyPressed(delta); //keypress check

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static glm::vec3 light(0, 25, -10);
        static glm::vec3 lightCol(1, 1, 1);

        Objects[0]->SetColor(glm::vec4(lightCol, 1.0));
        Objects[0]->SetTransform(glm::translate(glm::mat4(1.0), light));

        renderer.RenderObjects(Objects, cam, proj, light, lightCol);

        // skybox here
        glDepthFunc(GL_LEQUAL);
        skyboxVAO.Bind();
        skyboxShader.Bind();
        skyboxShader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(cam.GetViewMatrix())));
        skyboxShader.SetUniformMat4f("u_proj", proj);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxShader.UnBind();
        skyboxVAO.Unbind();

        //imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test");
        ImGui::Text("Application Delta %.3f ms/frame (%.1f ms)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Position: x:%.3f  y:%.3f  z:%.1f ", cam.GetPos().x, cam.GetPos().y, cam.GetPos().z);
        ImGui::Text("Look Dir: x:%.3f y:%.3f z:%.3f", cam.GetFront().x, cam.GetFront().y, cam.GetFront().z);
        ImGui::Text("Cameramode Mode: %d", CurrentWindowMode);

        ImGui::ColorEdit3("Light Color: ", (float*) &lightCol);
        ImGui::DragFloat3("Light position", (float*) &light);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyPressed(float delta) {
    //camera movement
    if (CurrentWindowMode == MouseInputMode::CAMERA_MODE){
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
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cam.ProcessKeyboard(ROLLLEFT, delta);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cam.ProcessKeyboard(ROLLRIGHT, delta);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (CurrentWindowMode == WINDOW_MODE) {    //close
                //break; //comment out to close on esc
                std::cout << "Escape key pressed, closing window." << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            } else {    //leave cameramode
                ToggleMouseInputMode(window, CurrentWindowMode);
            }
            break;
        case GLFW_KEY_F11:
            toggleFullscreen();
            break;
        case GLFW_KEY_F:
            toggleFullscreen();
            break;
        }
    }
}

void MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) { //ignore mouseclicks on imgui
        return;
    }

    if (CurrentWindowMode == WINDOW_MODE){
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            ToggleMouseInputMode(window, CurrentWindowMode);
            firstMouse = true;
        }
    } else { //cameramode

    }
}

void MousePosCallBack(GLFWwindow* window, double xpos, double ypos) {
    if (CurrentWindowMode == WINDOW_MODE)
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

void toggleFullscreen() {
    static GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Get the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);   // Get monitor resolution

    if (isFullscreen) {
        glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
    } else {
        glfwGetWindowPos(window, &windowedX, &windowedY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    isFullscreen = !isFullscreen;
}