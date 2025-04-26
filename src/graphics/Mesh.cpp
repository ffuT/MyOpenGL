#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices) :
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
	m_indexCount(indices.size()) {

	m_VAO.Bind();
	m_EBO.Bind();
	m_VAO.AddVertexBuffer(m_VBOPos, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.AddVertexBuffer(m_VBONorm, 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.BindElementArrayBuffer(m_EBO);

	m_VAO.Unbind();
	m_EBO.Unbind();
}

void Mesh::Bind() const{
	m_VAO.Bind();
}

void Mesh::Unbind() const{
	m_VAO.Unbind();
}

void Mesh::Render() const{
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}
