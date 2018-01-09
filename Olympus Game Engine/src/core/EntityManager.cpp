#include "EntityManager.h"

EntityManager::EntityManager() {

}
void EntityManager::addEntity(Entity* entity) {
	m_entityList.push_back(entity);
}

Entity* EntityManager::getEntity(int ID) {
	for (auto entity : m_entityList) {
		if (entity->m_ID == ID)
			return entity;
	}
}

void EntityManager::render() {
	for (auto entity : m_entityList)
		entity->update();
}

void EntityManager::renderShadow() {
	for (auto entity : m_entityList)
		entity->updateShadow();
}

std::vector<Entity*> EntityManager::getEntityList() {
	return m_entityList;
}