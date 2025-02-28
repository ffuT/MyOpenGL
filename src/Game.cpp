#include "Game.h"

Game::Game(const  char* title) : m_title(title) {
    if (!glfwInit()) {
        std::cout << "error initializing glfw" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "Loading...", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cout << "error creating window" << std::endl;
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwMakeContextCurrent(window);

    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSwapInterval(USE_VSYNC); //vsyncs

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window); // Clear to black while initializing

    //glfw callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->MouseCallBack(window, button, action, mods);
        });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->MousePosCallBack(window, xpos, ypos);
        });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
        });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->FramebufferSizeCallBack(window, width, height);
        });

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

Game::~Game(){

}

void Game::Run(){
    Renderer renderer = Renderer();

    Sphere light = Sphere(1, 16, ShaderProgram::UnlitShader);
    m_objects.push_back(&light);

    Sphere sphere = Sphere(5, 32);
    sphere.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(-25.0, -5.0, -50.0)));
    sphere.SetColor(glm::vec4(1, 0, 0, 1));
    m_objects.push_back(&sphere);

    Sphere sphere2 = Sphere(5, 32);
    sphere2.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -5.0, -50.0)));
    sphere2.SetColor(glm::vec4(0, 1, 0, 1));
    m_objects.push_back(&sphere2);

    Sphere sphere3 = Sphere(5, 32);
    sphere3.SetTransform(glm::translate(glm::mat4(1.0), glm::vec3(25.0, -5.0, -50.0)));
    sphere3.SetColor(glm::vec4(0, 0, 1, 1));
    m_objects.push_back(&sphere3);

    //skybox
    VertexArray skyboxVAO;
    VertexBuffer skyboxVBO(108 * sizeof(float), skyboxes::skyboxVertices);
    skyboxVAO.Bind();
    skyboxVAO.AddVertexBuffer(skyboxVBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    Shader skyboxShader("res/shaders/Skybox.shader");
    skyboxVAO.Unbind();
    skyboxVBO.Unbind();
    GLuint cubemapTexture = skyboxes::loadCubemap(skyboxes::Space);
    
    auto last = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    glfwSetWindowTitle(window, m_title);
    while (!glfwWindowShouldClose(window)) { // window/game loop
        //update values
        last = now;
        now = std::chrono::high_resolution_clock::now();
        delta = (now - last).count();

        keyPressed(delta); //keypress check

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static glm::vec3 light(0, 25, -10);
        static glm::vec3 lightCol(1, 1, 1);

        m_objects[0]->SetColor(glm::vec4(lightCol, 1.0));
        m_objects[0]->SetTransform(glm::translate(glm::mat4(1.0), light));

        renderer.RenderObjects(m_objects, cam, m_proj, light, lightCol);

        // skybox here
        glDepthFunc(GL_LEQUAL);
        skyboxVAO.Bind();
        skyboxShader.Bind();
        skyboxShader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(cam.GetViewMatrix())));
        skyboxShader.SetUniformMat4f("u_proj", m_proj);
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
        ImGui::Text("Cameramode Mode: %d", currentWindowMode);

        ImGui::ColorEdit3("Light Color: ", (float*)&lightCol);
        ImGui::DragFloat3("Light position", (float*)&light);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Game::keyPressed(float delta) {
    //camera movement
    if (currentWindowMode == MouseInputMode::CAMERA_MODE) {
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

void Game::ToggleMouseInputMode(GLFWwindow* window, MouseInputMode& mode) {
    if (mode == MouseInputMode::WINDOW_MODE) {
        mode = CAMERA_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        mode = MouseInputMode::WINDOW_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
};

void Game::toggleFullscreen() {
    static GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Get the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);   // Get monitor resolution

    if (IS_FULLSCREEN) {
        glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
    }
    else {
        glfwGetWindowPos(window, &windowedX, &windowedY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    IS_FULLSCREEN = !IS_FULLSCREEN;
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (currentWindowMode == MouseInputMode::WINDOW_MODE) {    //close
                //break; //comment out to close on esc
                std::cout << "Escape key pressed, closing m_window." << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            else {    //leave cameramode
                ToggleMouseInputMode(window, currentWindowMode);
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

void Game::MouseCallBack(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) { //ignore mouseclicks on imgui
        return;
    }

    if (currentWindowMode == MouseInputMode::WINDOW_MODE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            ToggleMouseInputMode(window, currentWindowMode);
            firstMouse = true;
        }
    }
    else { //cameramode

    }
}

void Game::MousePosCallBack(GLFWwindow* window, double xpos, double ypos) {
    if (currentWindowMode == MouseInputMode::WINDOW_MODE)
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

void Game::FramebufferSizeCallBack(GLFWwindow* window, int width, int height){
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}
