#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &m_ID);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FrameBuffer::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::attachTexture(GLenum attachment,unsigned int textureID) {
	glFramebufferTexture2D(GL_FRAMEBUFFER,attachment, GL_TEXTURE_2D, textureID, 0);
}

void FrameBuffer::attachTexture(GLenum attachment, const Texture& texture) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.GetID(), 0);
}

void FrameBuffer::attachRenderBuffer(GLenum attachment, unsigned int rboID) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rboID);
}

void FrameBuffer::setReadDrawBuffers(GLenum draw, GLenum read) {
	glDrawBuffer(draw);
	glReadBuffer(read);
}
