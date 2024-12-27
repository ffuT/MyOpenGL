#pragma once
#include <GLFW/glfw3.h>

//temp shader stuff
enum ShaderProgram {
    Skybox,
    NewShader,
    BasicShader
};

//input stuff
enum MouseInputMode {
    WINDOW_MODE,
    CAMERA_MODE
};

void ToggleMouseInputMode(GLFWwindow* window, MouseInputMode& mode) {
    if (mode == WINDOW_MODE) {
        mode = CAMERA_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        mode = WINDOW_MODE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
};


//sphere creation
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

float* CreateSphereNormals(const float* points, const int PointAmount) {
    float* normals = new float[3 * PointAmount * PointAmount]; // 3 components per normal (x, y, z)

    int index = 0;
    for (int i = 0; i < PointAmount; i++) {
        for (int j = 0; j < PointAmount; j++) {
            // Fetch the point position
            float x = points[3 * (i * PointAmount + j)];
            float y = points[3 * (i * PointAmount + j) + 1];
            float z = points[3 * (i * PointAmount + j) + 2];

            // Normalize the normal (it’s the same as the position for a unit sphere)
            float length = sqrt(x * x + y * y + z * z);
            normals[index++] = x / length; // Normalized x
            normals[index++] = y / length; // Normalized y
            normals[index++] = z / length; // Normalized z
        }
    }
    return normals;
}


//skybox stuff      
std::vector<std::string> faces{
    "res/textures/skybox/right.jpg",
    "res/textures/skybox/left.jpg",
    "res/textures/skybox/top.jpg",
    "res/textures/skybox/bottom.jpg",
    "res/textures/skybox/front.jpg",
    "res/textures/skybox/back.jpg"
};

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
};

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
