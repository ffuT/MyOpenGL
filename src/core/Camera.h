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
public:
    Camera();
    ~Camera();
    glm::vec3 GetPos() const;
    glm::vec3 GetFront() const;
    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(const Camera_Movement& direction, const float& delta);
    void ProcessMouse(const float& xOffset, const float& yOffset);

private:
    const float m_MovementSpeed = 0.1;  // x speed * 1000 = movement in 3d space a second
    const float m_MouseSensitivity = 3.09; //s1mple sens

    glm::vec3 m_Position = glm::vec3(0.0);
    glm::quat m_Orientation = glm::quat(1.0, 0.0, 0.0, 0.0);
};