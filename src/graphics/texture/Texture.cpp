#include "Texture.h"

Texture::Texture(int width, int height,
	GLenum internalFormat,
	GLenum format,
	GLenum type) : m_Width(width), m_Height(height) {

	glGenTextures(1, &m_ID);
	this->Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::UnBind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameter(GLenum param, GLenum value) const {
	glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::setParameterfv(GLenum param, const float* value) const {
	glTexParameterfv(GL_TEXTURE_2D, param, value);
}
