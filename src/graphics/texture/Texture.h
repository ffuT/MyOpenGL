#pragma once
#include <GL/glew.h>

class Texture {
	public:
		Texture(int width, int height,
			GLenum internalFormat,
			GLenum format,
			GLenum type);

		~Texture();
		void Bind() const;
		void UnBind() const;

		void setParameter(GLenum param, GLenum value) const;
		void setParameterfv(GLenum param, const float* value) const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline unsigned int GetID() const { return m_ID; }

private:
	unsigned int m_ID;
	int m_Width, m_Height;
};