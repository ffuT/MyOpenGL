#pragma once
#include <GL/glew.h>

#include "Texture.h"

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void UnBind();

	void attachTexture(GLenum attachment, unsigned int textureID);
	void attachTexture(GLenum attachment, const Texture& texture);
	void attachRenderBuffer(GLenum attachment, unsigned int rboID);
	void setReadDrawBuffers(GLenum draw, GLenum read);

	unsigned int GetID() const { return m_ID; }

private:
	unsigned int m_ID;
};