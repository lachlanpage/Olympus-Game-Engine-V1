#include "Entity.h"

int Entity::uniqueIDCounter = 0;

Entity::Entity(){
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;
}

Entity::Entity(glm::vec3 position, GraphicsComponent* graphics) {
	m_position = position;
	m_graphics = graphics;
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;	
}

Entity::Entity(GraphicsComponent* graphics) {
	m_graphics = graphics;
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;
}
Entity::~Entity(){}
glm::vec3 Entity::getPosition() { return m_position; }
void Entity::setPosition(glm::vec3 position) { m_position = position; }
void Entity::update(){
	if (m_graphics != nullptr) {
		m_graphics->update(*this);
	}
}