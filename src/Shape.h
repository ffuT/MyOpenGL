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
	glm::vec4 Color = glm::vec4(1.0);

	ShaderProgram ShaderName;

	VertexArray m_VAO = VertexArray();
	VertexBuffer m_VBOPos;
	VertexBuffer m_VBONorm;
	ElementArrayBuffer m_EBO;

	glm::mat4 m_Scale = glm::mat4(1.0);
	glm::mat4 m_Rotation = glm::mat4(1.0);
	glm::mat4 m_Transform = glm::mat4(1.0);

public:
	Shape(const std::vector<float>& vertices,
		const std::vector<float>& normals, 
		const std::vector<unsigned int>& indices,
		const ShaderProgram& shaderName);

	Shape(const std::vector<float>& vertices,
		const std::vector<float>& normals, 
		const std::vector<unsigned int>& indices,
		const ShaderProgram& shaderName,
		const glm::vec4& color);
	
	virtual ~Shape();
	
	virtual void RenderStart();
	virtual void RenderStop();

	virtual ShaderProgram GetShader();
	virtual ShaderProgram GetShaderName();
	virtual glm::mat4 GetModelMatrix();
	virtual glm::vec4 GetColor();

	virtual void SetTransform(glm::mat4 transform);

	virtual void SetColor(const glm::vec4 color);
};