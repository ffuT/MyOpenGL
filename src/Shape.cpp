#include "Shape.h"
#include <iostream>

Shape::Shape(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices,
	const ShaderProgram& shaderName)
	: Vertices(vertices), Normals(normals), Indices(indices),
		VBOPos(vertices.size() * sizeof(float), vertices.data()),
		VBONorm(normals.size() * sizeof(float), normals.data()),
		EBO(indices.size() * sizeof(unsigned int), indices.data()),
		ShaderName(shaderName) {

	VAO.Bind();
	EBO.Bind();
	VAO.AddVertexBuffer(VBOPos, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	VAO.AddVertexBuffer(VBONorm, 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	VAO.BindElementArrayBuffer(EBO);

	VAO.Unbind();
	EBO.Unbind();
}

Shape::~Shape(){
}

void Shape::RenderStart(){
	VAO.Bind();
}

void Shape::RenderStop() {
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}

glm::mat4 Shape::GetModelMatrix(){
	return Transform * (Rotation * Scale);
}