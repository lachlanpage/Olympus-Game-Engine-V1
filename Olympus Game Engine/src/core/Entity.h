#ifndef ENTITY_H 
#define ENTITY_H 
#include <glm/glm.hpp>
#include "../components/GraphicsComponent.h"

class GraphicsComponent;

class Entity {
public:
	static int uniqueIDCounter;
	int m_ID;

	Entity();
	Entity(GraphicsComponent*);
	Entity(glm::vec3 position, GraphicsComponent* graphics);
	~Entity();
	glm::vec3 getPosition();
	void setPosition(glm::vec3);
	void update();

private:
	glm::vec3 m_position;
	GraphicsComponent* m_graphics;
};

#endif