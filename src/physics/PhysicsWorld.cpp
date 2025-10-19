#include "PhysicsWorld.h"

void PhysicsWorld::AddBody(RigidBody* body) {
	m_bodies.push_back(body);
}

void PhysicsWorld::Step(float deltaTime) {
	for (RigidBody* body : m_bodies) {
		body->Integrate(deltaTime);
	}
}