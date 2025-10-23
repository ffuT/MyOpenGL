#pragma once
#include <glm/glm.hpp>

enum class ColliderType {
	Sphere,
	Plane,
	Mesh,
};

struct RigidBody {
	bool isStatic = false;
	float mass = 1.0;
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 velocity = glm::vec3(0.0);
	glm::vec3 forces = glm::vec3(0.0);

	float restitution = 0.75f; // bounciness
	ColliderType colliderType = ColliderType::Sphere;
	float radius = 1.0f;
	glm::vec3 halfExtents = glm::vec3(1.0f);

	void AddForce(const glm::vec3& f) { forces += f; };
	void ClearForces() { forces = glm::vec3(0.0f); };
};