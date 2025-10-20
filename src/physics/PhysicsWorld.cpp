#include "PhysicsWorld.h"
#include <iostream>

void PhysicsWorld::AddBody(RigidBody* body) {
	m_bodies.push_back(body);
}

void PhysicsWorld::RemoveBody(RigidBody* body) {
	if (!body) return;
	m_bodies.erase(std::remove(m_bodies.begin(), m_bodies.end(), body), m_bodies.end());
}

void PhysicsWorld::Step(float deltaTime) {
	for (RigidBody* body : m_bodies) {
		if (body->isStatic) continue;

		// apply forces
		body->AddForce(glm::vec3(0.0f, -9.81f * body->mass, 0.0f)); // gravity

		// euler integration
		Intergrate(body, deltaTime);

		// collision detection + roslution ground
		float groundY = -150.0f; // ground level
		if ((body->position.y - body->radius) < groundY) {
			body->position.y = groundY + body->radius;

			// bounce
			if (body->velocity.y < 0.0f){
				body->velocity.y *= -body->restitution;
				if (std::abs(body->velocity.y) < 0.5f) {
					body->velocity.y = 0.0f;
				}
			}

			// friction
			glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 tanVel = body->velocity - glm::dot(body->velocity, normal) * normal;

			float speed = glm::length(tanVel);
			if (speed > 0.0f) {
				glm::vec3 dir = tanVel / speed;
				float frictCoef = 0.5f; // simple constant friction coefficient
				float friction = frictCoef * 9.81f * body->mass * deltaTime;

				float reduce = std::min(friction, speed);
				body->velocity -= dir * reduce;

				if (glm::length(body->velocity) < 0.05f)
					body->velocity.x = body->velocity.z = 0.0f;
			}
		}
		//clear forces
		body->ClearForces();
	}
	// collision detection + resolution between bodies
	for (size_t i = 0; i < m_bodies.size(); ++i) {
		for (size_t j = i + 1; j < m_bodies.size(); ++j) {
			RigidBody* A = m_bodies[i];
			RigidBody* B = m_bodies[j];
			if (IsColliding(*A, *B))
				ResolveCollision(*A, *B);
		}
	}
}

void PhysicsWorld::Intergrate(RigidBody* body, float delta) {
	glm::vec3 acceleration = body->forces / body->mass;
	body->velocity += acceleration * delta;
	body->position += body->velocity * delta;
}

void PhysicsWorld::ResolveCollision(RigidBody& a, RigidBody& b) {
	glm::vec3 normal = glm::normalize(b.position - a.position);
	float distance = glm::length(b.position - a.position);
	float penetration = (a.radius + b.radius) - distance;

	// position correction
	if (penetration > 0.0f) {
		float totalMass = (a.isStatic ? 0.0f : a.mass) + (b.isStatic ? 0.0f : b.mass);
		if (totalMass == 0.0f)
			return;

		float moveA = (a.isStatic ? 0.0f : b.mass / totalMass);
		float moveB = (b.isStatic ? 0.0f : a.mass / totalMass);
		a.position -= normal * penetration * moveA;
		b.position += normal * penetration * moveB;
	}

	//reflect velocities
	glm::vec3 relativeVelocity = b.velocity - a.velocity;
	float velAlongNormal = glm::dot(relativeVelocity, normal);
	if(velAlongNormal > 0.0f)
		return; // moving apart
	
	float e = std::min(a.restitution, b.restitution); // bounciness
	float j = -(1 + e) * velAlongNormal;
	j /= (a.isStatic ? 0.0f : 1.0f / a.mass) + (b.isStatic ? 0.0f : 1.0f / b.mass);

	glm::vec3 impulse = j * normal;
	if(!a.isStatic)
		a.velocity -= (1.0f / a.mass) * impulse;
	if (!b.isStatic)
		b.velocity += (1.0f / b.mass) * impulse;
}

bool PhysicsWorld::IsColliding(RigidBody& a, RigidBody& b) {
	if (a.isStatic == true && b.isStatic == true)
		return false;

	if (a.colliderType == ColliderType::Sphere && b.colliderType == ColliderType::Sphere) {
		float dist = glm::length(a.position - b.position);
		float minDist = a.radius + b.radius;
		if (dist < minDist)
			return true;
	} 
	// later add more stuff
		
	return false;
}
