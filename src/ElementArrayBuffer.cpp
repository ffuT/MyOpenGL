#include "ElementArrayBuffer.h"
#include <GL/glew.h>

ElementArrayBuffer::ElementArrayBuffer(unsigned int size, const void* data){
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementArrayBuffer::~ElementArrayBuffer(){
	glDeleteBuffers(1, &m_ID);
}

void ElementArrayBuffer::Bind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void ElementArrayBuffer::Unbind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
