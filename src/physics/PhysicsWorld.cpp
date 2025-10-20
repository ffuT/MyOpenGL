#include "PhysicsWorld.h"
#include <iostream>

void PhysicsWorld::AddBody(RigidBody* body) {
	m_bodies.push_back(body);
}

void PhysicsWorld::Step(float deltaTime) {
	for (RigidBody* body : m_bodies) {
		if (body->isStatic) return;
		body->AddForce(glm::vec3(0.0f, -9.81f, 0.0f)); // gravity

		body->velocity += body->forces / body->mass * deltaTime;
		body->position += body->velocity * deltaTime;

		body->ClearForces(); // reset force
	}
}