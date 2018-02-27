#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H
#include "Component.h"
#include <bullet/btBulletDynamicsCommon.h>
class PhysicsComponent : public Component {
public:
	PhysicsComponent(glm::vec3, btCollisionShape*, int);
	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);
	btRigidBody* getBody();

private:
	btCollisionShape* collisionShape;
	btDefaultMotionState* fallMotionState;
	btRigidBody* fallRigidBody;
};
#endif