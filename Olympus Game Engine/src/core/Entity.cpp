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

Entity::Entity(glm::vec3 position) {
	m_position = position;
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

	for (auto component : m_components)
		component->update(*this);

	is_selected = false;
}

void Entity::updateShadow() {
	for (auto component : m_components)
		component->renderShadow(*this);
}


void Entity::addComponent(Component* component) {
	//work around to send important init data instead of using constructors
	component->postInit(*this);
	m_components.push_back(component);
	
}