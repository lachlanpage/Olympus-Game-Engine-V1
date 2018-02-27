#include "PhysicsEngine.h"

#include <iostream>
PhysicsEngine::PhysicsEngine() {
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	btCollisionShape* fallShape = new btSphereShape(1);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody);

	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);

	//calculate rigid body and add to world
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);

	//for (int i = 0; i < 300; i++) {

	//	dynamicsWorld->stepSimulation(1 / 60.f, 10);

	//	btTransform trans;
	//	fallRigidBody->getMotionState()->getWorldTransform(trans);

	//	std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
	//}
}

void PhysicsEngine::Update() {

	//update eeach step
	dynamicsWorld->stepSimulation(1 / 60.f, 10);

	btTransform trans;
	//first elem

	for (rigidBodyListIterator = rigidBodyList.begin(); rigidBodyListIterator != rigidBodyList.end(); rigidBodyListIterator++) {
		Entity *entity = rigidBodyListIterator->second.first;
		btRigidBody* rigidBody = rigidBodyListIterator->second.second;

		//yaw = yaxis
		//pitch = xaxis
		//roll = z axis
		float RAD2DEG = 180.0 / 3.141592653589793238463;
		entity->setPosition(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		entity->setRotation(glm::vec3(trans.getRotation().getX()*RAD2DEG, trans.getRotation().getY()*RAD2DEG, trans.getRotation().getZ()*RAD2DEG));
		rigidBody->getMotionState()->getWorldTransform(trans);
	}
	//for (auto body : rigidBodyList) {
	//	std::cout << body.first() << std::endl;
		//body.first->setPosition(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		//body.second->getMotionState()->getWorldTransform(trans);
	//}

	//std::cout << rigidBodyList.size() << std::endl;
	

	

}

void PhysicsEngine::addBodies(std::vector<Entity*> entityList) {

	for (auto entity : entityList) {
		if (entity->GetComponent<PhysicsComponent>() != nullptr) {
			int id = entity->m_ID;
			rigidBodyListIterator = rigidBodyList.find(id);
			if (rigidBodyListIterator != rigidBodyList.end()) {
				//weve found it
				//do whatever i want here
			}
			else {
				dynamicsWorld->addRigidBody(entity->GetComponent<PhysicsComponent>()->getBody());
				rigidBodyList.insert(std::make_pair(id, std::make_pair(entity, entity->GetComponent<PhysicsComponent>()->getBody())));
			}
		}

	}
}

btDiscreteDynamicsWorld* PhysicsEngine::getDynamicsWorld() {
	return dynamicsWorld;
}