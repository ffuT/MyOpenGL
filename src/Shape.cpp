#include "Shape.h"
#include <GL/glew.h>

Shape::Shape(const std::vector<float>& vertices,
	const std::vector<float>& normals,
	const std::vector<unsigned int>& indices,
	const ShaderProgram& shaderName)
	: Vertices(vertices), Normals(normals), Indices(indices),
		VBOPos(Vertices.size() * sizeof(float), Vertices.data()),
		VBONorm(Normals.size() * sizeof(float), Normals.data()),
		EBO(Indices.size() * sizeof(unsigned int), Indices.data()),
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

void Shape::Render(){
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}

glm::mat4 Shape::GetModelMatrix()
{
	return glm::mat4(1.0); //Transform * Rotation * Scale
}