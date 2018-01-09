#ifndef ENTITYMANAGER_H 
#define ENTITYMANAGER_H 
#include "Entity.h"
#include <vector>
class EntityManager {
public:
	EntityManager();
	void addEntity(Entity*);
	Entity* getEntity(int ID);
	void render();
	void renderShadow();
	std::vector<Entity*> getEntityList();
private:
	std::vector<Entity*> m_entityList;
};
#endif