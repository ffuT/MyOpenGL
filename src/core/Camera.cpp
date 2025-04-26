#include "Camera.h"

/*
    I dont understand quaternions... thank you chatgpt and deepseek
*/

Camera::Camera() {
}

Camera::~Camera() {
}

glm::vec3 Camera::GetPos() const {
    return m_position;
}

glm::vec3 Camera::GetFront() const {
    return m_orientation * glm::vec3(0.0, 0.0, -1.0);
}

glm::mat4 Camera::GetViewMatrix() const {
        glm::vec3 front = m_orientation * glm::vec3(0.0, 0.0, -1.0);
        glm::vec3 up = m_orientation * glm::vec3(0.0, 1.0, 0.0);
        return glm::lookAt(m_position, m_position + front, up);
}

void Camera::ProcessKeyboard(const Camera_Movement& direction, const float& delta) {
    float moveSpeed = m_movementSpeed / 1000000 * (2 * m_sprint + 1);
    glm::vec3 MovementDir = glm::vec3(0.0);

    // Calculate local front, right, and up vectors based on the current orientation
    glm::vec3 front = m_orientation * glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 right = m_orientation * glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 up = m_orientation * glm::vec3(0.0, 1.0, 0.0);

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
        float rollSpeed = m_rollSpeed / 1000000; // adjust roll speed
        float rollAngle = rollSpeed * delta;

        if (direction == ROLLLEFT)
            rollAngle = -rollAngle;

        glm::vec3 front = glm::normalize(m_orientation * glm::vec3(0.0, 0.0, -1.0));
        glm::quat rollQuat = glm::angleAxis(glm::radians(rollAngle), front);

        m_orientation = rollQuat * m_orientation;
        m_orientation = glm::normalize(m_orientation);
        return;
    }

    float velocity = delta * moveSpeed;
    m_position += glm::normalize(MovementDir) * velocity;
}

void Camera::ProcessMouse(const float& xOffset, const float& yOffset) {
    glm::vec3 localRight = m_orientation * glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 localUp = m_orientation * glm::vec3(0.0, 1.0, 0.0);

    glm::quat yawQuat = glm::angleAxis(glm::radians(-xOffset * m_mouseSensitivity), localUp); // Rotate around local up
    glm::quat pitchQuat = glm::angleAxis(glm::radians(yOffset * m_mouseSensitivity), localRight); // Rotate around local right

    m_orientation = yawQuat * pitchQuat * m_orientation;
    m_orientation = glm::normalize(m_orientation);
}
