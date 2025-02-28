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
    glm::vec3 GetPos();
    glm::vec3 GetFront();
    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement movement, float delta);
    void ProcessMouse(float xOffset, float yOffset);

private:
    const float m_MovementSpeed = 0.1;  // x speed * 1000 = movement in 3d space a second
    const float m_MouseSensitivity = 2;

    glm::vec3 m_Position = glm::vec3(0.0);
    glm::quat m_Orientation = glm::quat(1.0, 0.0, 0.0, 0.0);

};