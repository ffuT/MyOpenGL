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
#include "Extras.h" 

enum MouseInputMode {
	WINDOW_MODE,
	CAMERA_MODE
};

class Game {
public:
    Game(const char* title);
    ~Game();
    void Run();

private:
    //program params
    const float YAW = 0.022f, PITCH = 0.022f; //same turn speed as CS2, UE5 default = 0.07
    unsigned int WIDTH = 1280, HEIGHT = 720;

    //program variables
    bool USE_VSYNC = false;
    bool IS_FULLSCREEN = false;
    bool firstMouse = true;

    float lastX = 400.0f;
    float lastY = 300.0f;
    int windowedX = 0, windowedY = 0, windowedWidth = 0, windowedHeight = 0; // save windowed mode state

    MouseInputMode currentWindowMode = WINDOW_MODE;
    GLFWwindow* window;
    Camera cam;

    const char* m_title;

    float delta = 0.0f;
    std::vector<Shape*> m_objects;
    glm::mat4 m_proj = glm::perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

    void toggleFullscreen();
    void keyPressed(float delta);
    void ToggleMouseInputMode(GLFWwindow* window, MouseInputMode& mode);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MouseCallBack(GLFWwindow* window, int button, int action, int mods);
    void MousePosCallBack(GLFWwindow* window, double xpos, double ypos);
    void FramebufferSizeCallBack(GLFWwindow* window, int width, int height);
};
