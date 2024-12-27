#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "ShaderManager.h"

class Shape{
protected:	
	std::vector<float> Vertices;
	std::vector<float> Normals;
	std::vector<unsigned int> Indices;
	
	ShaderProgram ShaderName;

	VertexArray VAO = VertexArray();
	VertexBuffer VBOPos;
	VertexBuffer VBONorm;
	ElementArrayBuffer EBO;
public:
	glm::mat4 Scale = glm::mat4(1.0);
	glm::mat4 Rotation = glm::mat4(1.0);
	glm::mat4 Transform = glm::mat4(1.0);

	Shape(const std::vector<float>& vertices,
		const std::vector<float>& normals, 
		const std::vector<unsigned int>& indices,
		const ShaderProgram& shaderName);
	
	virtual ~Shape();
	
	virtual void RenderStart();
	virtual void RenderStop();

	virtual glm::mat4 GetModelMatrix();
	virtual ShaderProgram GetShaderName();
};