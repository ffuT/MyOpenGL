#include "Skybox.h"

Skybox::Skybox(){
    m_skyboxVAO.Bind();
    m_skyboxVBO.Bind();
    m_skyboxVAO.AddVertexBuffer(m_skyboxVBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_skyboxVAO.Unbind();
    m_skyboxVBO.Unbind();
}

Skybox::~Skybox(){
    glDeleteTextures(1, &m_cubemapTexture);
}

void Skybox::Bind(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);

    m_skyboxVAO.Bind();
}

void Skybox::UnBind(){
    m_skyboxVAO.Unbind();
}
