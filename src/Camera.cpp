#include "Camera.h"

Camera::Camera() :
	m_Position(glm::vec3(0.0)),
	m_Front(glm::vec3(0.0, 0.0, 1.0)),
	m_Up(glm::vec3(0.0, 1.0, 0.0)),
	m_Right(glm::vec3(1.0, 0.0, 0.0)),
	m_WorldUp(m_Up)

{

}

Camera::~Camera(){

}

void Camera::Update(){
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
   
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta){
    float velocity = m_MovementSpeed * delta;
    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
    //temp
    if (direction == rotRIGHT)
        m_Yaw += 2.2;
    if (direction == rotLEFT)
        m_Yaw -= 2.2;
    if (direction == rotUP){
        m_Pitch += 2.2;
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
    }
    if (direction == rotDOWN){
        m_Pitch -= 2.2;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    Update();
}
