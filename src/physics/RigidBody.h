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

<<<<<<< HEAD
	float restitution = 0.7f; // bounciness
=======
	float restitution = 0.75f; // bounciness
>>>>>>> 7a96299e74eded38d589449c5fd8b6736814f5a3
	ColliderType colliderType = ColliderType::Sphere;
	float radius = 1.0f;
	glm::vec3 halfExtents = glm::vec3(1.0f);

	void AddForce(const glm::vec3& f) { forces += f; };
	void ClearForces() { forces = glm::vec3(0.0f); };
};
