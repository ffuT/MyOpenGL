#include "ShadowMap.h"

ShadowMap::ShadowMap(int Width, int Height) : 
	shadowMapWidth(Width), shadowMapHeight(Height),
	m_shadowTexture(Texture(shadowMapWidth, shadowMapHeight,
		GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT)) {
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_shadowTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_shadowTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_shadowTexture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	m_shadowTexture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	m_shadowTexture.setParameterfv(GL_TEXTURE_BORDER_COLOR, clampColor);

	m_FBO.Bind();
    m_FBO.attachTexture(GL_DEPTH_ATTACHMENT, m_shadowTexture);
	m_FBO.setReadDrawBuffers(GL_NONE, GL_NONE); // no color buffer
	m_FBO.UnBind();  
}

ShadowMap::~ShadowMap() {

}
