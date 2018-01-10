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

	int is_selected;

	Entity();
	Entity(GraphicsComponent*);
	Entity(glm::vec3);
	Entity(glm::vec3 position, GraphicsComponent* graphics);
	~Entity();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	void addComponent(Component*);
	void setPosition(glm::vec3);
	void setScale(glm::vec3);
	void setRotation(glm::vec3);
	void update();
	void updateShadow();

	template<typename ComponentType>
	ComponentType* GetComponent() {
		for (auto component : m_components) {
			if (ComponentType* comp = dynamic_cast<ComponentType*>(component)) {
				return comp;
			}
		}
	}

private:
	std::vector<Component*> m_components;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	GraphicsComponent* m_graphics;
};

#endif