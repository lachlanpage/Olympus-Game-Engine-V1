#include "Entity.h"

int Entity::uniqueIDCounter = 0;

Entity::Entity(){
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;
}

Entity::Entity(glm::vec3 position, GraphicsComponent* graphics) {
	m_position = position;
	m_scale = glm::vec3(1, 1, 1);
	m_rotation = glm::vec3(0, 0, 0);
	m_graphics = graphics;
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;	

	is_selected = false;
}

Entity::Entity(glm::vec3 position) {
	m_position = position;
	m_scale = glm::vec3(1, 1, 1);
	m_rotation = glm::vec3(0, 0, 0);
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;

	is_selected = false;
}

Entity::Entity(GraphicsComponent* graphics) {
	m_graphics = graphics;
	m_scale = glm::vec3(1, 1, 1);
	m_rotation = glm::vec3(0, 0, 0);
	m_ID = uniqueIDCounter;
	uniqueIDCounter++;

	is_selected = false;
}
Entity::~Entity(){}
glm::vec3 Entity::getPosition() { return m_position; }
glm::vec3 Entity::getRotation() { return m_rotation; }
glm::vec3 Entity::getScale()    {return m_scale;}

void Entity::setPosition(glm::vec3 position) { m_position = position; }
void Entity::setScale(glm::vec3 scale)       { m_scale = scale; }
void Entity::setRotation(glm::vec3 rotation) { m_rotation = rotation; }

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