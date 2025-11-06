#pragma once

#include <GL/glew.h>

#include "FrameBuffer.h"
#include "Texture.h"

class ShadowMap {
public:	
	ShadowMap(int Width, int Height);
	~ShadowMap();

	inline void Bind() { m_FBO.Bind(); }

	inline unsigned int getWidth() const { return shadowMapWidth; }
	inline unsigned int getHeight() const { return shadowMapHeight; }	
	inline const Texture& getShadowTexture() const { return m_shadowTexture; }

private:
	unsigned int shadowMapWidth, shadowMapHeight;
	FrameBuffer m_FBO;
	Texture m_shadowTexture;
};
