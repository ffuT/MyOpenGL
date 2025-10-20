#pragma once
#include <vector>
#include "RigidBody.h"

class PhysicsWorld {
public:
	void AddBody(RigidBody* body);
	void RemoveBody(RigidBody* body);
	void Step(float deltaTime);


private:
	void Intergrate(RigidBody* body, float delta);
	void ResolveCollision(RigidBody& a, RigidBody& b);
	bool IsColliding(RigidBody& a, RigidBody& b);

	std::vector<RigidBody*> m_bodies;
};