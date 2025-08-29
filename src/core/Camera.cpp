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
    float moveSpeed = m_movementSpeed / 1000000 * (1.66f * m_sprint + 1);
    glm::vec3 MovementDir = glm::vec3(0.0);

    // Calculate local vectors
	glm::vec3 front, right, up;
    right = m_orientation * glm::vec3(1.0, 0.0, 0.0);
    if(USE_QUAT_ROTATION){
        front = m_orientation * glm::vec3(0.0, 0.0, -1.0);
        up = m_orientation * glm::vec3(0.0, 1.0, 0.0);
    } else {
		front = glm::normalize(glm::cross(right, glm::vec3(0.0, -1.0, 0.0)));
        up = glm::vec3(0.0, 1.0, 0.0);
    }

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
        if(!USE_QUAT_ROTATION)
			return;

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
    if (USE_QUAT_ROTATION) {
        glm::vec3 localRight = m_orientation * glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 localUp = m_orientation * glm::vec3(0.0, 1.0, 0.0);

        glm::quat yawQuat = glm::angleAxis(glm::radians(-xOffset * m_mouseSensitivity), localUp); // Rotate around local up
        glm::quat pitchQuat = glm::angleAxis(glm::radians(yOffset * m_mouseSensitivity), localRight); // Rotate around local right

        m_orientation = yawQuat * pitchQuat * m_orientation;
        m_orientation = glm::normalize(m_orientation);
    } else {
        m_yaw -= xOffset * m_mouseSensitivity;
        m_pitch += yOffset * m_mouseSensitivity;

        // Clamp pitch
        if (m_pitch > 89.0)
            m_pitch = 89.0;
        if (m_pitch < -89.0)
            m_pitch = -89.0;

        // Convert updated Euler angles to quaternion
        glm::quat qPitch = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0, 1, 0));

        // Combine yaw and pitch (no roll here)
        m_orientation = qYaw * qPitch;
        m_orientation = glm::normalize(m_orientation);
    }
}
