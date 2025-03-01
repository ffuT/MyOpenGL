#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "ShaderManager.h"

class Shape{
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

	virtual void Render() const;

	virtual void SetTransform(const glm::mat4 transform);
	virtual void SetColor(const glm::vec4 color);
	virtual void SetSpecular(const float spec);
	virtual void SetTextID(char* name);
	
	virtual float GetSpecular() const;
	virtual ShaderProgram GetShader() const;
	virtual ShaderProgram GetShaderName() const;
	virtual glm::mat4 GetModelMatrix() const;
	virtual glm::mat4 GetTransform() const;
	virtual glm::vec4 GetColor() const;
	virtual const char* GetTextID() const;

	std::string ObjectTextID;
protected:
	std::vector<float> m_Vertices;
	std::vector<float> m_Normals;
	std::vector<unsigned int> m_Indices;
	glm::vec4 m_Color = glm::vec4(1.0);
	float m_SpecularStrenght = 0.5;

	ShaderProgram m_ShaderName;

	VertexArray m_VAO;
	VertexBuffer m_VBOPos;
	VertexBuffer m_VBONorm;
	ElementArrayBuffer m_EBO;

	glm::mat4 m_Scale = glm::mat4(1.0);
	glm::mat4 m_Rotation = glm::mat4(1.0);
	glm::mat4 m_Transform = glm::mat4(1.0);

};