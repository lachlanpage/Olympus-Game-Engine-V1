#ifndef ENTITYMANAGER_H 
#define ENTITYMANAGER_H 
#include "Entity.h"
#include <vector>

#include "../components/CubeGraphicsComponent.h"	
#include "../components/LightComponent.h"
#include "../components/DirectionalLightComponent.h"
#include "../components/QuadGraphicsComponent.h"
#include "../components/PlaneGraphicsComponent.h"
#include "../components/ModelComponent.h"

class EntityManager {
public:
	EntityManager();
	void addEntity(Entity*);
	Entity* getEntity(int ID);
	void render();
	void renderShadow();
	void renderLight();
	std::vector<Entity*> getEntityList();
private:
	std::vector<Entity*> m_entityList;
};
#endif