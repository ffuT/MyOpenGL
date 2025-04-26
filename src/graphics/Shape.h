#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementArrayBuffer.h"
#include "ShaderManager.h"
#include "Mesh.h"

class Shape{
public:
	Shape(Mesh* mesh);
	
	virtual ~Shape();

	virtual void Render() const;

	virtual void SetTransform(const glm::mat4 transform);
	virtual void SetScale(const glm::mat4 transform);
	virtual void SetRotation(const glm::mat4 transform);
	virtual void SetColor(const glm::vec4 color);
	virtual void SetSpecular(const float spec);
	virtual void SetTextID(const char* name);
	
	virtual float GetSpecular() const;
	virtual glm::mat4 GetModelMatrix() const;
	virtual glm::mat4 GetTransform() const;
	virtual glm::mat4 GetScale() const;
	virtual glm::mat4 GetRotation() const;
	virtual glm::vec4 GetColor() const;
	virtual const char* GetTextID() const;

	std::string m_ObjectTextID;
protected:
	Mesh* m_mesh;
	glm::vec4 m_Color = glm::vec4(1.0);
	float m_SpecularStrenght = 0.5;

	glm::mat4 m_Scale = glm::mat4(1.0);
	glm::mat4 m_Rotation = glm::mat4(1.0);
	glm::mat4 m_Transform = glm::mat4(1.0);
};