#pragma once
#include <vector>
#include "RigidBody.h"

class PhysicsWorld {
public:
	void AddBody(RigidBody* body);
	void Step(float deltaTime);

	std::vector<RigidBody*> m_bodies;
private:
};