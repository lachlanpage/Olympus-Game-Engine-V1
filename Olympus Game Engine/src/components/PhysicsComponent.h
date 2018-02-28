#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H
#include "Component.h"
#include <bullet/btBulletDynamicsCommon.h>
class PhysicsComponent : public Component {
public:
	PhysicsComponent(glm::vec3 pos, btCollisionShape* colType, int mass);
	//used to apply initial force to a body
	PhysicsComponent(glm::vec3 pos, btCollisionShape* colType, int mass, float force);
	btRigidBody* getBody();
	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);
private:
	btCollisionShape* collisionShape;
	btDefaultMotionState* fallMotionState;
	btRigidBody* fallRigidBody;
};
#endif