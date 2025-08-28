#include "DebugCrosshair.h"

DebugCrosshair::DebugCrosshair() : m_VBO(18 * sizeof(float), m_Vertices){
    m_VAO.Bind();
    m_VAO.AddVertexBuffer(m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_VAO.Unbind();
    m_VBO.Unbind();
}

DebugCrosshair::~DebugCrosshair(){

}

void DebugCrosshair::Update(const Camera& cam){
    glm::vec3 cameraPos = cam.GetPos() + cam.GetFront();
    const float LINE_LENGTH = 0.025f;
    // X-axis
    m_Vertices[0] = cameraPos.x; m_Vertices[1] = cameraPos.y; m_Vertices[2] = cameraPos.z;
    m_Vertices[3] = cameraPos.x + LINE_LENGTH; m_Vertices[4] = cameraPos.y; m_Vertices[5] = cameraPos.z;
    // Y-axis
    m_Vertices[6] = cameraPos.x; m_Vertices[7] = cameraPos.y; m_Vertices[8] = cameraPos.z;
    m_Vertices[9] = cameraPos.x; m_Vertices[10] = cameraPos.y + LINE_LENGTH; m_Vertices[11] = cameraPos.z;
    // Z-axis
    m_Vertices[12] = cameraPos.x; m_Vertices[13] = cameraPos.y; m_Vertices[14] = cameraPos.z;
    m_Vertices[15] = cameraPos.x; m_Vertices[16] = cameraPos.y; m_Vertices[17] = cameraPos.z + LINE_LENGTH;
    m_VBO.Bind();
    m_VBO.UpdateBuffer(0, 18 * sizeof(float), m_Vertices);
    m_VBO.Unbind();
}

void DebugCrosshair::BindVAO() const {
    m_VAO.Bind();
}

void DebugCrosshair::UnBindVAO() const{
    m_VAO.Unbind();
}
