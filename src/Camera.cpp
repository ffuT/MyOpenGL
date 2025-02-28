#include "Camera.h"
#include <iostream>

/*
    I dont understand quaternions... thank you chatgpt and deepseek
*/

Camera::Camera() {
}

Camera::~Camera() {
}

glm::vec3 Camera::GetPos() {
    return m_Position;
}

glm::vec3 Camera::GetFront(){
    return m_Orientation * glm::vec3(0.0, 0.0, -1.0);
}

glm::mat4 Camera::GetViewMatrix() {
        glm::vec3 front = m_Orientation * glm::vec3(0.0, 0.0, -1.0);
        glm::vec3 up = m_Orientation * glm::vec3(0.0, 1.0, 0.0);
        return glm::lookAt(m_Position, m_Position + front, up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta) {
    float moveSpeed = m_MovementSpeed / 1000000;
    glm::vec3 MovementDir = glm::vec3(0.0);

    // Calculate local front, right, and up vectors based on the current orientation
    glm::vec3 front = m_Orientation * glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 right = m_Orientation * glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 up = m_Orientation * glm::vec3(0.0, 1.0, 0.0);

    if (direction == FORWARD)
        MovementDir += front;
    if (direction == BACKWARD)
        MovementDir -= front;
    if (direction == LEFT)
        MovementDir -= right;
    if (direction == RIGHT)
        MovementDir += right;
    if (direction == UP)
        MovementDir += up;
    if (direction == DOWN)
        MovementDir -= up;

    if (direction == ROLLLEFT || direction == ROLLRIGHT) {
        float rollSpeed = moveSpeed * 1.5; // adjust roll speed
        float rollAngle = rollSpeed * delta;

        if (direction == ROLLLEFT)
            rollAngle = -rollAngle;

        glm::vec3 front = glm::normalize(m_Orientation * glm::vec3(0.0, 0.0, -1.0));
        glm::quat rollQuat = glm::angleAxis(glm::radians(rollAngle), front);

        m_Orientation = rollQuat * m_Orientation;
        m_Orientation = glm::normalize(m_Orientation);
        return;
    }

    float velocity = delta * moveSpeed;
    m_Position += glm::normalize(MovementDir) * velocity;
}

void Camera::ProcessMouse(float xOffset, float yOffset) {
    glm::vec3 localRight = m_Orientation * glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 localUp = m_Orientation * glm::vec3(0.0, 1.0, 0.0);

    glm::quat yawQuat = glm::angleAxis(glm::radians(-xOffset * m_MouseSensitivity), localUp); // Rotate around local up
    glm::quat pitchQuat = glm::angleAxis(glm::radians(yOffset * m_MouseSensitivity), localRight); // Rotate around local right

    m_Orientation = yawQuat * pitchQuat * m_Orientation;
    m_Orientation = glm::normalize(m_Orientation);
}
