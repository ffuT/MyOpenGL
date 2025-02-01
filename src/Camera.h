#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

/*
    I dont understand quaternions... thank you chatgpt and deepseek
*/

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROLLLEFT,
    ROLLRIGHT
};

class Camera {
private:
    const glm::vec3 m_WorldUp = glm::vec3(0.0, 1.0, 0.0);
    const float m_MovementSpeed = 0.05;
    const float m_MouseSensitivity = 2;

    glm::vec3 m_Position = glm::vec3(0.0);
    glm::quat m_Orientation = glm::quat(1.0, 0.0, 0.0, 0.0);

public:
    Camera();
    ~Camera();
    glm::vec3 GetPos();
    glm::vec3 GetFront();
    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement movement, float delta);
    void ProcessMouse(float xOffset, float yOffset);
};