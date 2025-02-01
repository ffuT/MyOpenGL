#include "Shape.h"
#include <iostream>

Shape::Shape(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices,
	const ShaderProgram& shaderName)
	: m_Vertices(vertices), m_Normals(normals), m_Indices(indices),
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
	m_ShaderName(shaderName) {

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
	: m_Vertices(vertices), m_Normals(normals), m_Indices(indices),
	m_VBOPos(vertices.size() * sizeof(float), vertices.data()),
	m_VBONorm(normals.size() * sizeof(float), normals.data()),
	m_EBO(indices.size() * sizeof(unsigned int), indices.data()),
	m_ShaderName(shaderName), m_Color(color) {

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

void Shape::Render() {
	m_VAO.Bind();
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	m_VAO.Unbind();
}

ShaderProgram Shape::GetShader(){
	return m_ShaderName;
}

glm::vec4 Shape::GetColor(){
	return m_Color;
}

float Shape::GetSpecular(){
	return m_SpecularStrenght;
}

void Shape::SetTransform(glm::mat4 transform){
	m_Transform = transform;
}

void Shape::SetColor(const glm::vec4 color){
	m_Color = color;
}

glm::mat4 Shape::GetModelMatrix(){
	return m_Transform * (m_Rotation * m_Scale);
}

ShaderProgram Shape::GetShaderName(){
	return m_ShaderName;
}
