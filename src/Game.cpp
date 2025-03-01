#include "Game.h"

Game::Game(const  char* title) : TITLE(title) {
    if (!glfwInit()) {
        std::cout << "error initializing glfw" << std::endl;
        exit(-1);
    }

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Loading...", NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        std::cout << "error creating window" << std::endl;
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwMakeContextCurrent(m_window);

    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSwapInterval(USE_VSYNC); //vsyncs

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window); // Clear to black while initializing

    //glfw callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->MouseCallBack(window, button, action, mods);
        });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->MousePosCallBack(window, xpos, ypos);
        });
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
        });
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->FramebufferSizeCallBack(window, width, height);
        });

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

Game::~Game(){

}

void Game::Run(){
    Renderer renderer = Renderer();

    Sphere light = Sphere(2, 16, ShaderProgram::UnlitShader);
    m_objects.push_back(&light);

    Sphere sphere = Sphere(5, 32);
    sphere.SetTextID((char*) "sphere 1");
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

    //skybox needs abstraction
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
    glfwSetWindowTitle(m_window, TITLE);
    while (!glfwWindowShouldClose(m_window)) { // window/game loop
        //update values
        last = now;
        now = std::chrono::high_resolution_clock::now();
        m_delta = (now - last).count();

        static glm::vec3 light(0, 92, -25);
        static glm::vec3 lightCol(1, 1, 1);

        keyPressed(m_delta); //keypress check

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Render(renderer, lightCol, light);

        // skybox here
        glDepthFunc(GL_LEQUAL);
        skyboxVAO.Bind();
        skyboxShader.Bind();
        skyboxShader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_cam.GetViewMatrix())));
        skyboxShader.SetUniformMat4f("u_proj", m_proj);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxShader.UnBind();
        skyboxVAO.Unbind();

        RenderImGui(lightCol, light);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Game::Render(Renderer& renderer, glm::vec3& lightcol, glm::vec3& lightpos) {
    m_objects[0]->SetColor(glm::vec4(lightcol, 1.0));
    m_objects[0]->SetTransform(glm::translate(glm::mat4(1.0), lightpos));

    renderer.RenderObjects(m_objects, m_cam, m_proj, lightpos, lightcol);
}

void Game::RenderImGui(glm::vec3& lightcol, glm::vec3& lightpos){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGui");
    ImGui::Text("Application Delta %.3f ms/frame (%.1f ms)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Position: x:%.3f  y:%.3f  z:%.1f ", m_cam.GetPos().x, m_cam.GetPos().y, m_cam.GetPos().z);
    ImGui::Text("Look Dir: x:%.3f y:%.3f z:%.3f", m_cam.GetFront().x, m_cam.GetFront().y, m_cam.GetFront().z);
    ImGui::Text("Camera Mode: %s", m_currentWindowMode ? "true" : "false");

    ImGui::NewLine();
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Settings");

    if (ImGui::Button("Fullscreen"))
        toggleFullscreen();
    ImGui::SameLine();
    ImGui::Text(": %s", IS_FULLSCREEN ? "on" : "off");
   
    if (ImGui::Button("VSync")) {
        USE_VSYNC = !USE_VSYNC;
        glfwSwapInterval(USE_VSYNC);
    }
    ImGui::SameLine();
    ImGui::Text(": %s", USE_VSYNC ? "on" : "off");
    ImGui::NewLine();
    
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Scene");
    ImGui::ColorEdit3("Light Color: ", (float*)&lightcol);
    ImGui::DragFloat3("Light position", (float*)&lightpos);

    ImGui::Spacing();

    static int selectedSphereIndex = -1;
    if (ImGui::BeginCombo("Object", selectedSphereIndex >= 0 ? ("Object " + std::to_string(selectedSphereIndex) + ": " + m_objects[selectedSphereIndex]->GetTextID()).c_str()  : "Objects")) {
        for (int i = 1; i < m_objects.size(); i++) { // Skip light sphere at index 0
            std::string itemLabel = "Index " + std::to_string(i) + ": " + m_objects[i]->GetTextID();
            bool isSelected = (selectedSphereIndex == i);
            if (ImGui::Selectable(itemLabel.c_str(), isSelected))
                selectedSphereIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selectedSphereIndex >= 0 && selectedSphereIndex < m_objects.size()) {
        Sphere* selectedSphere = static_cast<Sphere*>(m_objects[selectedSphereIndex]);

        float specular = selectedSphere->GetSpecular();
        glm::vec4 color = selectedSphere->GetColor();
        glm::vec3 position = glm::vec3(selectedSphere->GetTransform()[3]); // Get translation from matrix

        ImGui::DragFloat("Specular", (float*)&specular, 0.001f, 0, 1);
        ImGui::ColorEdit3("Color", (float*)&color);
        ImGui::DragFloat3("Position", (float*)&position, 0.1f);

        selectedSphere->SetColor(color);
        selectedSphere->SetTransform(glm::translate(glm::mat4(1.0), position));
        selectedSphere->SetSpecular(specular);
    }

    ImGui::NewLine();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::keyPressed(const float& delta) {
    //camera movement
    if (m_currentWindowMode == MouseInputMode::CAMERA_MODE) {
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_cam.ProcessKeyboard(FORWARD, delta);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            m_cam.ProcessKeyboard(BACKWARD, delta);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_cam.ProcessKeyboard(LEFT, delta);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            m_cam.ProcessKeyboard(RIGHT, delta);
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_cam.ProcessKeyboard(UP, delta);
        if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
            m_cam.ProcessKeyboard(DOWN, delta);
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            m_cam.ProcessKeyboard(DOWN, delta);
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_cam.ProcessKeyboard(ROLLLEFT, delta);
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            m_cam.ProcessKeyboard(ROLLRIGHT, delta);
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
        glfwSetWindowMonitor(m_window, nullptr, m_windowedX, m_windowedY, m_windowedWidth, m_windowedHeight, 0);
    }
    else {
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    IS_FULLSCREEN = !IS_FULLSCREEN;
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (m_currentWindowMode == MouseInputMode::WINDOW_MODE) {    //close
                //break; //comment out to close on esc
                std::cout << "Escape key pressed, closing m_window." << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            else {    //leave cameramode
                ToggleMouseInputMode(window, m_currentWindowMode);
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

    if (m_currentWindowMode == MouseInputMode::WINDOW_MODE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            ToggleMouseInputMode(window, m_currentWindowMode);
            FIRST_MOUSE = true;
        }
    }
    else { //cameramode

    }
}

void Game::MousePosCallBack(GLFWwindow* window, double xpos, double ypos) {
    if (m_currentWindowMode == MouseInputMode::WINDOW_MODE)
        return;

    if (FIRST_MOUSE) {
        m_lastX = xpos;
        m_lastY = ypos;
        FIRST_MOUSE = false;
    }

    float xOffset = (xpos - m_lastX) * YAW;
    float yOffset = (m_lastY - ypos) * PITCH;

    m_lastX = xpos;
    m_lastY = ypos;

    m_cam.ProcessMouse(xOffset, yOffset);
}

void Game::FramebufferSizeCallBack(GLFWwindow* window, int width, int height){
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}
