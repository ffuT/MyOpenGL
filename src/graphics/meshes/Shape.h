#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "RigidBody.h"
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
	
	// physics
	RigidBody& GetRigidBody() { return m_RigidBody; }
	const RigidBody& GetRigidBody() const;
	void syncPhysicsToTransform();
	void syncTransformToPhysics();

	// transforms
	glm::mat4 GetModelMatrix() const;
	void UpdateModelMatrix() const;
	glm::mat4 GetTransform() const;
	glm::mat4 GetRotation() const;
	glm::mat4 GetScale() const;

	void SetPosition(const glm::vec3 pos);
	void SetRotation(const glm::mat4 transform);
	void SetScale(const glm::vec3 scale);
	void SetScale(const float scale);

	// properties
	glm::vec4 GetColor() const;
	float GetSpecular() const;
	
	void SetColor(const glm::vec4 color);
	void SetSpecular(const float spec);
	
	Mesh* GetMesh() const;
	void SetMesh(Mesh* mesh);
	
	const char* GetTextID() const;
	void SetTextID(const char* name);

private:
	Mesh* m_mesh;
	glm::vec4 m_Color = glm::vec4(1.0);
	float m_SpecularStrength = 0.5;
	std::string m_ObjectTextID;

	RigidBody m_RigidBody;

	glm::vec3 m_Scale = glm::vec3(1.0);
	glm::mat4 m_Rotation = glm::mat4(1.0);

	mutable glm::mat4 m_cachedModelMatrix = glm::mat4(1.0);
	mutable bool m_dirty = true;
};