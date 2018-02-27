#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(glm::vec3 positionBody, btCollisionShape* collisionShapeType, int objectMass) {
	collisionShape = collisionShapeType;
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(positionBody.x, positionBody.y, positionBody.z)));
	btScalar mass = objectMass;
	btVector3 fallInertia(20, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
}

void PhysicsComponent::update(Entity& entity){
	
}

btRigidBody* PhysicsComponent::getBody() {
	return fallRigidBody;
}
void PhysicsComponent::renderShadow(Entity& entity){}
void PhysicsComponent::postInit(Entity& entity){}