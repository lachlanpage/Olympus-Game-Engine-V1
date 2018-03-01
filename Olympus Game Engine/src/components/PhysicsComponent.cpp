#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(glm::vec3 positionBody, btCollisionShape* collisionShapeType, int objectMass) {
	//standard rigid body component generator
	collisionShape = collisionShapeType;
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(positionBody.x, positionBody.y, positionBody.z)));
	btScalar mass = objectMass;
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
}

PhysicsComponent::PhysicsComponent(glm::vec3 positionBody, btCollisionShape* collisionShapeType, int objectMass, float force) {
	//used to apply initial force to a body
	collisionShape = collisionShapeType;
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(positionBody.x, positionBody.y, positionBody.z)));
	btScalar mass = objectMass;
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);

	btVector3 dir = btVector3(Camera::Instance()->getDirection().x, Camera::Instance()->getDirection().y, Camera::Instance()->getDirection().z) * force;
	fallRigidBody->applyCentralImpulse(dir);
}

PhysicsComponent::PhysicsComponent(glm::vec3 positionBody, btCollisionShape* collisionShapeType) {
	//creates a fixed/static body.
	collisionShape = collisionShapeType;
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(positionBody.x, positionBody.y, positionBody.z)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, collisionShape, btVector3(0,0,0));
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
}



btRigidBody* PhysicsComponent::getBody() {
	return fallRigidBody;
}

void PhysicsComponent::update(Entity& entity) {}
void PhysicsComponent::renderShadow(Entity& entity){}
void PhysicsComponent::postInit(Entity& entity){}