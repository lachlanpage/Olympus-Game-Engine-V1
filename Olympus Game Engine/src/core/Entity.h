#ifndef ENTITY_H 
#define ENTITY_H 
#include <glm/glm.hpp>
#include <vector>
#include "../components/GraphicsComponent.h"
#include "../components/Component.h"

class GraphicsComponent;
class Component;

class Entity {
public:
	static int uniqueIDCounter;
	int m_ID;

	Entity();
	Entity(GraphicsComponent*);
	Entity(glm::vec3);
	Entity(glm::vec3 position, GraphicsComponent* graphics);
	~Entity();
	glm::vec3 getPosition();
	void addComponent(Component*);
	void setPosition(glm::vec3);
	void update();
	void updateShadow();

private:
	std::vector<Component*> m_components;
	glm::vec3 m_position;
	GraphicsComponent* m_graphics;
};

#endif