#pragma once
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

#include "Skybox.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Extras.h" 

class Game {
public:
    Game(const char* title);
    ~Game();
    void Run();

private:
    void RenderImGui(Renderer& renderer);
    void RenderImGuiData();
    void RenderImGuiSettings(Renderer& renderer);
    void RenderImGuiSceneControl();
    
    void UpdateXHair(VertexBuffer& XhairVBO);

    void toggleFullscreen();
    void ToggleMouseInputMode(GLFWwindow* window, MouseInputMode& mode); // for camera movment and ui control
    
    void keyPressed(const float& delta); // update camera pos
    
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
    void MousePosCallBack(GLFWwindow* window, double xpos, double ypos);
    void FramebufferSizeCallBack(GLFWwindow* window, int width, int height); // update window variables on rezise

    const char* TITLE;
    float YAW = 0.022f, PITCH = 0.022f; // camera turn speed, same as CS2, UE5 default = 0.07
    unsigned int WIDTH = 1280, HEIGHT = 720;

    bool USE_VSYNC = false;
    bool IS_FULLSCREEN = false;
    bool FIRST_MOUSE = true;
    bool USE_DEBUG_XHAIR = true;

    float m_lastX = 400.0f;
    float m_lastY = 300.0f;
    int m_windowedX = 0, m_windowedY = 0, m_windowedWidth = 0, m_windowedHeight = 0; // save windowed mode state

    MouseInputMode m_currentWindowMode = WINDOW_MODE;
    GLFWwindow* m_window;
    Camera m_cam;

    float m_delta = 0.0f;
    std::vector<Shape> m_objects;
    std::vector<Light> m_lights;
    std::vector<Mesh> m_meshes;
    glm::mat4 m_proj = glm::perspective(glm::radians(70.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);

    float m_xHairVertices[18] = { // for debug crosshair render
        -0.02f, 0.0f,  0.0f,  0.02f, 0.0f,  0.0f,
         0.0f, -0.02f, 0.0f,  0.0f,  0.02f, 0.0f,
         0.0f,  0.0f, -0.02f, 0.0f,  0.0f,  0.02f
    };
};
