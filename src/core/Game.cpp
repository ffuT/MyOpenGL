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
    glfwSwapInterval(USE_VSYNC); // vsyncs

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window); // Clear to black while initializing

    // glfw callbacks
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
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

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
    renderer.SetRenderShader(ShaderProgram::NewShader);

    m_meshes.reserve(10); // IMPORTANT!!!, to avoid vector move shenanigans very bandaid fix
    Mesh spheremesh = Mesh(CreateSphere(1, 32), CreateSphere(1, 32), CreateSphereNormals(CreateSphere(1, 32), 32), CreateSphereIndices(32));
    Mesh Bananmesh = Mesh::LoadMeshFromFile("res/meshes/banana.obj");
    Mesh Quadmesh = Mesh(CreateQuadGrid(10), CreateQuadGrid(10), CreateQuadGridNormals(10), CreateQuadGridIndices(10));
    glm::mat4 terrainmodel = glm::mat4(1.0f);
	terrainmodel = glm::scale(terrainmodel, glm::vec3(200.0f, 1.0f, 200.0f));
	terrainmodel = glm::translate(terrainmodel, glm::vec3(-0.5f, -150.0f, -0.5f)); // center terrain at origin

	m_meshes.push_back(Quadmesh);   // index 0 terrain
    m_meshes.push_back(spheremesh); // index 1 sphere
	m_meshes.push_back(Bananmesh);  // index 2 banana

    Skybox skybox = Skybox(skyboxes::Space);
    DebugCrosshair DebugXhair = DebugCrosshair();

    // sphere obejct for rendering lights always index 0
    Shape pointLight = Shape(&m_meshes[1]);
    pointLight.SetTextID("PointLight");
    m_objects.push_back(pointLight);
    
    // constant light source(s)
    Light directional = Light(glm::vec3(0.44, -0.46, 0.78), glm::vec3(1.0), 1.0f, 1);
    m_lights.push_back(directional);

    std::chrono::nanoseconds seed = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::srand(seed.count());
    for (int i = 0; i < 50; i++) { // bunch of random spheres for visualitation and performance check
        bool isbanana = (std::rand() % 100) < 5; // 5% chance for banana mesh
        m_objects.push_back(Shape(&m_meshes[isbanana+1]));
        m_objects[i + 1].SetScale(2.5 + std::rand() % 15);
        
        float f2 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float f1 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float f3 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        
        m_objects[i + 1].SetColor(glm::vec4(f1, f2, f3, 1)); // random color

        int max = 100;
        int min = -100;
        // random pos
        m_objects[i + 1].SetTransform(glm::translate(glm::mat4(1.0),
            glm::vec3(min + (std::rand() % (max - min + 1)),
                min + (std::rand() % (max - min + 1)),
                min + (std::rand() % (max - min + 1)))));
    }
    
    auto last = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    glfwSetWindowTitle(m_window, TITLE);
    while (!glfwWindowShouldClose(m_window)) { // window/game loop
        last = now;
        now = std::chrono::high_resolution_clock::now();
        m_delta = (now - last).count();
        m_lifetime += m_delta;

        keyPressed(m_delta); // keypress handling

		// render to shadowmap + update shadowmap matrix
        if (DYNAMIC_LIGHT_CYCLE) {
			float speed = glm::radians(m_LightRotationSpeed); // degrees per second
			float angle = speed * (float)m_delta / 1000000000;
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
			m_lights[0].position = glm::vec3(rot * glm::vec4(m_lights[0].position, 1.0f));
			m_lights[0].position = glm::normalize(m_lights[0].position); // keep directional light at infinity
        }
        renderer.RenderShadowMap(m_objects, m_lights);

        // render screen 
        glViewport(0,0 ,WIDTH, HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear
        renderer.RenderTerrain(m_meshes[0], terrainmodel, m_cam, m_proj, m_lights);
        renderer.RenderSkybox(skybox, m_cam, m_proj);
        renderer.RenderObjects(m_objects, m_lights, m_cam, m_proj);

		//maybe move skybox and crosshair renderer

		if (USE_DEBUG_XHAIR) { // render crosshair
            DebugXhair.Update(m_cam);
            renderer.RenderCrosshair(DebugXhair, m_cam, m_proj);
        }
        RenderImGui(renderer);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Game::RenderImGui(Renderer& renderer) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    
    ImGui::NewFrame();

    RenderImGuiData();
    RenderImGuiSettings(renderer);
    RenderImGuiSceneControl();

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderImGuiData() {
    ImGui::Begin("ImGui");
    ImGui::Text("Application Delta %.3f ms/frame (%.1f)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Text("Position: ");  // Camerea position display 
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
    ImGui::Text("x:%.3f", m_cam.GetPos().x);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
    ImGui::Text(" y:%.3f", m_cam.GetPos().y);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.33f, 1.0f, 1.0f)); // Blue
    ImGui::Text(" z:%.1f", m_cam.GetPos().z);
    ImGui::PopStyleColor();     // cam pos end

    ImGui::Text("Look Dir: x:%.3f y:%.3f z:%.3f", m_cam.GetFront().x, m_cam.GetFront().y, m_cam.GetFront().z);
    ImGui::Text("Camera Mode: %s", m_currentWindowMode ? "true" : "false");

    ImGui::NewLine();
}

void Game::RenderImGuiSettings(Renderer& renderer) {
    ImGui::Text("Settings");

	if (ImGui::Button("Fullscreen"))        //toggle fullscreen
        toggleFullscreen();
    ImGui::SameLine();
    ImGui::Text(": %s", IS_FULLSCREEN ? "on" : "off");

	if (ImGui::Button("Vsync")) {           //toggle vsync
        USE_VSYNC = !USE_VSYNC;
        glfwSwapInterval(USE_VSYNC);
    }
    ImGui::SameLine();
    ImGui::Text(": %s", USE_VSYNC ? "on" : "off");

	if (ImGui::Button("Unlocked Cam"))      // toggle camera mode
        m_cam.USE_QUAT_ROTATION = !m_cam.USE_QUAT_ROTATION;
    ImGui::SameLine();
    ImGui::Text(": %s", m_cam.USE_QUAT_ROTATION ? "on" : "off");

	if (ImGui::Button("Debug Xhair"))       // toggle debug crosshair
        USE_DEBUG_XHAIR = !USE_DEBUG_XHAIR;
    ImGui::SameLine();
    ImGui::Text(": %s", USE_DEBUG_XHAIR ? "on" : "off");

	if (ImGui::Button("Dynamic Light cycle"))       // toggle dynamic light cycle
        DYNAMIC_LIGHT_CYCLE = !DYNAMIC_LIGHT_CYCLE;
    ImGui::SameLine();
    ImGui::Text(": %s", DYNAMIC_LIGHT_CYCLE ? "on" : "off");

	ImGui::SliderFloat("Rotation Speed", &m_LightRotationSpeed, -180.0f, 180.0f);

    ImGui::Spacing();
    ImGui::Text("Shader Program ");         // Shader Program changer
    if (ImGui::Button("NewShader"))
        renderer.SetRenderShader(ShaderProgram::NewShader);
    ImGui::SameLine();
    if (ImGui::Button("Wireframe"))
        renderer.SetRenderShader(ShaderProgram::WireframeShader);
    ImGui::SameLine();
    if (ImGui::Button("Unlit"))
        renderer.SetRenderShader(ShaderProgram::UnlitShader);
    ImGui::NewLine();
}

void Game::RenderImGuiSceneControl() {
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Scene");
    ImGui::Spacing();

    if (ImGui::Button("Add Object")) {
        m_objects.push_back(Shape(&m_meshes[1]));
    }
    ImGui::Spacing();

    static int selectedSphereIndex = -1;
    if (ImGui::Button("Delete Object")) {   // delete selected Sphere
        if (selectedSphereIndex > 0) { // > 0 because 0 cant be deleted
            m_objects.erase(m_objects.begin() + selectedSphereIndex);
            if (selectedSphereIndex > 1) // > 1 because i dont wanna edit sphere 0
                selectedSphereIndex--;
        }
    }
    ImGui::Spacing();
    
    if (ImGui::BeginCombo("Object", selectedSphereIndex >= 0 ? ("Object " + std::to_string(selectedSphereIndex) + ": " + m_objects[selectedSphereIndex].GetTextID()).c_str() : "Objects")) {
        for (int i = 1; i < m_objects.size(); i++) { // Skip light sphere at index 0
            std::string itemLabel = "Index " + std::to_string(i) + ": " + m_objects[i].GetTextID();
            bool isSelected = (selectedSphereIndex == i);
            if (ImGui::Selectable(itemLabel.c_str(), isSelected))
                selectedSphereIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selectedSphereIndex >= 0 && selectedSphereIndex < m_objects.size()) {
        Shape& selectedSphere = m_objects[selectedSphereIndex];

        float specular = selectedSphere.GetSpecular();
        glm::vec4 color = selectedSphere.GetColor();
        glm::vec3 position = glm::vec3(selectedSphere.GetTransform()[3]); // Get translation from matrix

        glm::mat4 scaleMatrix = selectedSphere.GetScale(); // Get scale matrix
        glm::vec3 scale = glm::vec3(glm::length(scaleMatrix[0]), glm::length(scaleMatrix[1]), glm::length(scaleMatrix[2]));

        ImGui::DragFloat("Specular", &specular, 0.001f, 0, 1);
        ImGui::ColorEdit3("Color", (float*)&color);
        ImGui::DragFloat3("Position", (float*)&position, 0.5f);
        ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);

        scaleMatrix = glm::scale(glm::mat4(1.0), scale); // Create a new scale matrix

        selectedSphere.SetColor(color);
        selectedSphere.SetSpecular(specular);

        glm::mat4 transform = glm::translate(glm::mat4(1.0), position);
        selectedSphere.SetTransform(transform);
        selectedSphere.SetScale(scaleMatrix);
    }
    ImGui::NewLine();

    if (ImGui::Button("Add Light")) {
        m_lights.push_back(Light(glm::vec3(0.0), glm::vec3(1.0), 0.5));
    }
    ImGui::Spacing();

    static int selectedLightIndex = -1;  // Currently selected light
    if (ImGui::Button("Delete Light")) {   // delete selected light
        if (selectedLightIndex > 0) {
            m_lights.erase(m_lights.begin() + selectedLightIndex);
            selectedLightIndex--;
        }
    }

    if (ImGui::BeginCombo("Light Source", selectedLightIndex >= 0 ? ("Light " + std::to_string(selectedLightIndex)).c_str() : "Lights")) {
        for (int i = 0; i < m_lights.size(); i++) {
            std::string itemLabel = "Light " + std::to_string(i);
            bool isSelected = (selectedLightIndex == i);
            if (ImGui::Selectable(itemLabel.c_str(), isSelected))
                selectedLightIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selectedLightIndex >= 0 && selectedLightIndex < m_lights.size()) {
        Light& selectedLight = m_lights[selectedLightIndex];

        ImGui::ColorEdit3("Color ", (float*)&selectedLight.color);
        ImGui::DragFloat("Intensity ", &selectedLight.intensity, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat3("Position ", (float*)&selectedLight.position, 0.5f);
    }
    ImGui::NewLine();
}

void Game::keyPressed(const float& delta) {
    // camera movement
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
        if (glfwGetKey(m_window, GLFW_KEY_C) || // C or Ctrl
            glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            m_cam.ProcessKeyboard(DOWN, delta);
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_cam.ProcessKeyboard(ROLLLEFT, delta);
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            m_cam.ProcessKeyboard(ROLLRIGHT, delta);
        (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // faster movement
            ? m_cam.m_sprint = true : m_cam.m_sprint = false;
    }
}

void Game::ToggleMouseInputMode(GLFWwindow* window, MouseInputMode& mode) {
    if (mode == MouseInputMode::WINDOW_MODE) {
        mode = CAMERA_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        mode = MouseInputMode::WINDOW_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
};

void Game::toggleFullscreen() {
    static GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Get the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);   // Get monitor resolution

    if (!IS_FULLSCREEN) { // save pos and size when not fullscreen
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else { // set to saved pos
        glfwSetWindowMonitor(m_window, nullptr, m_windowedX, m_windowedY, m_windowedWidth, m_windowedHeight, 0);
    }
    IS_FULLSCREEN = !IS_FULLSCREEN;
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (m_currentWindowMode == MouseInputMode::WINDOW_MODE) {
                std::cout << "Escape key pressed, closing m_window." << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            } else {
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
    if (ImGui::GetIO().WantCaptureMouse) { // ignore mouseclicks on imgui
        return;
    }
    if (m_currentWindowMode == MouseInputMode::WINDOW_MODE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            ToggleMouseInputMode(window, m_currentWindowMode);
            FIRST_MOUSE = true;
        }
    } else { 
        // cameramode
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
