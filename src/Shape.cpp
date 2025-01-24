#include "Shape.h"
#include <iostream>

Shape::Shape(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices,
	const ShaderProgram& shaderName)
	: Vertices(vertices), Normals(normals), Indices(indices),
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
		ShaderName(shaderName) {

	m_VAO.Bind();
	m_EBO.Bind();
	m_VAO.AddVertexBuffer(m_VBOPos, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.AddVertexBuffer(m_VBONorm, 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.BindElementArrayBuffer(m_EBO);

	m_VAO.Unbind();
	m_EBO.Unbind();
}

Shape::Shape(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices,
	const ShaderProgram& shaderName,
	const glm::vec4& color)
	: Vertices(vertices), Normals(normals), Indices(indices),
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
	ShaderName(shaderName), Color(color) {

	m_VAO.Bind();
	m_EBO.Bind();
	m_VAO.AddVertexBuffer(m_VBOPos, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.AddVertexBuffer(m_VBONorm, 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_VAO.BindElementArrayBuffer(m_EBO);

	m_VAO.Unbind();
	m_EBO.Unbind();
}


Shape::~Shape(){
}

void Shape::RenderStart(){
	m_VAO.Bind();
}

void Shape::RenderStop() {
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	m_VAO.Unbind();
}

ShaderProgram Shape::GetShader(){
	return ShaderName;
}

glm::vec4 Shape::GetColor(){
	return Color;
}

void Shape::SetTransform(glm::mat4 transform){
	m_Transform = transform;
}

void Shape::SetColor(const glm::vec4 color){
	Color = color;
}

glm::mat4 Shape::GetModelMatrix(){
	return m_Transform * (m_Rotation * m_Scale);
}

ShaderProgram Shape::GetShaderName(){
	return ShaderName;
}
