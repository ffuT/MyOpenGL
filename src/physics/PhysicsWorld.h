#pragma once
#include <vector>
#include "RigidBody.h"

class PhysicsWorld {
public:
	void AddBody(RigidBody* body);
	void RemoveBody(RigidBody* body);
	void Step(float deltaTime);

private:
	void Integrate(RigidBody* body, float delta);
	void ResolveCollision(RigidBody& a, RigidBody& b, glm::vec3& normal, float& penetration);
	bool IsColliding(RigidBody& a, RigidBody& b, glm::vec3& normal, float& penetration);

	bool SphereSphereCol(RigidBody& a, RigidBody& b, glm::vec3& normal, float& penetration);
	bool SpherePlaneCol(RigidBody& a, RigidBody& b, glm::vec3& normal, float& penetration);
	bool MeshMeshCol(RigidBody& a, RigidBody& b, glm::vec3& normal, float& penetration);

	std::vector<RigidBody*> m_bodies;
};