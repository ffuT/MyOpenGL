#include "Camera.h"

Camera::Camera() :
	m_Position(glm::vec3(0.0)),
	m_Front(glm::vec3(0.0, 0.0, -1.0)),
	m_Up(glm::vec3(0.0, 1.0, 0.0)),
	m_Right(glm::vec3(1.0, 0.0, 0.0))
{

}

Camera::~Camera(){

}

glm::vec3 Camera::GetPos(){
    return m_Position;
}

void Camera::Update(){
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta){
    glm::vec3 MovementDir = glm::vec3(0.0);

    if (direction == FORWARD)
        MovementDir += m_Front;
    if (direction == BACKWARD)
        MovementDir -= m_Front;
    if (direction == LEFT)
        MovementDir -= m_Right;
    if (direction == RIGHT)
        MovementDir += m_Right;
    if (direction == UP)
        MovementDir += m_Up;
    if (direction == DOWN)
        MovementDir -= m_Up;

    float velocity = delta * m_MovementSpeed;
    m_Position += glm::normalize(MovementDir) * velocity;

    Update();
}

void Camera::ProcessMouse(float xOffset, float yOffset) {
    m_Yaw += xOffset * m_MouseSensitivity;
    m_Pitch += yOffset * m_MouseSensitivity;
    if (m_Pitch > 89.0f) 
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) 
        m_Pitch = -89.0f;
    Update();
}

