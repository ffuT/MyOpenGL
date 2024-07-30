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

const float YAW = -90.0f;
const float PITCH = 0.0f;
//const float SPEED = 2.5f;
//const float ZOOM = 45.0f;

class Camera{
public:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_MovementSpeed = 0.02;
    //float m_MouseSensitivity;
    //float m_Zoom;
    
public:
    Camera();
    ~Camera();

    void Update();
    void ProcessKeyboard(Camera_Movement movement, float delta);
    inline glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, (m_Position + m_Front), m_Up);}

};