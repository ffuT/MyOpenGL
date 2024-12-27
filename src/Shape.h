#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "Extras.h"

class Shape{
protected:	
	std::vector<float> Vertices;
	std::vector<float> Normals;
	std::vector<unsigned int> Indices;
	
	const ShaderProgram ShaderName;

	VertexArray VAO;
	VertexBuffer VBOPos;
	VertexBuffer VBONorm;
	ElementArrayBuffer EBO;

	glm::mat4 Scale = glm::mat4(1.0);
	glm::mat4 Rotation = glm::mat4(1.0);
	glm::mat4 Transform = glm::mat4(1.0);

public:
	Shape(const std::vector<float>& vertices,
		const std::vector<float>& normals, 
		const std::vector<unsigned int>& indices,
		const ShaderProgram& shaderName);
	virtual ~Shape();
	virtual void Render();
	virtual glm::mat4 GetModelMatrix();

};