#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    rotRIGHT,
    rotLEFT,
    rotUP,
    rotDOWN
};

class Camera{
public:
    const glm::vec3 m_WorldUp = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_MovementSpeed = 0.02;
    float m_MouseSensitivity = 2;
    //float m_Zoom;
    
public:
    Camera();
    ~Camera();

    glm::vec3 GetPos();

    void Update();
    void ProcessKeyboard(Camera_Movement movement, float delta);
    void ProcessMouse(float xOffset, float yOffset);
    inline glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, (m_Position + m_Front), m_Up);}

};