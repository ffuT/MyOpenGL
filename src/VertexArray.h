#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"

class VertexArray{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const VertexBuffer& vbo, GLuint index, GLint size,
		GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

	void BindElementArrayBuffer(const ElementArrayBuffer& ebo);

private:
	unsigned int m_ID;
};

