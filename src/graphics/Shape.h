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
	~Shape();

	void Render() const;

	void SetTransform(const glm::mat4 transform);
	void SetScale(const glm::mat4 scale);
	void SetScale(const float scale);
	void SetRotation(const glm::mat4 transform);
	void SetColor(const glm::vec4 color);
	void SetSpecular(const float spec);
	void SetTextID(const char* name);
	
	float GetSpecular() const;
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetTransform() const;
	glm::mat4 GetScale() const;
	glm::mat4 GetRotation() const;
	glm::vec4 GetColor() const;
	const char* GetTextID() const;
	
	std::string m_ObjectTextID;

private:
	Mesh* m_mesh;
	glm::vec4 m_Color = glm::vec4(1.0);
	
	float m_SpecularStrenght = 0.5;

	glm::mat4 m_Scale = glm::mat4(1.0);
	glm::mat4 m_Rotation = glm::mat4(1.0);
	glm::mat4 m_Transform = glm::mat4(1.0);

	mutable glm::mat4 m_cachedModelMatrix = glm::mat4(1.0);
	mutable bool m_dirty = true;
};