#pragma once
#include <glm/glm.hpp>

struct RigidBody {
	bool isStatic = false;
	float mass = 1.0;
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 velocity = glm::vec3(0.0);
	glm::vec3 forces = glm::vec3(0.0);

	void AddForce(const glm::vec3& f) { forces += f; };
	void ClearForces() { forces = glm::vec3(0.0f); };
	void Integrate(float dt) {
		if (isStatic) return;
		glm::vec3 acceleration = forces / mass;
		velocity += acceleration * dt;
		position += velocity * dt;
		ClearForces();
	};
};