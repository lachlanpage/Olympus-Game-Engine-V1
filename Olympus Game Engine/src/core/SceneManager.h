#ifndef SCENEMANAGER_H 
#define SCENEMANAGER_H
#include "../utilities/tinyxml2.h"
#include <iostream>
#include <string>

#include "Entity.h"
#include "EntityManager.h"

class SceneManager {
public:
	SceneManager(const char* filepath, EntityManager* manager);
	
private:
	tinyxml2::XMLDocument doc;
	void parseScene();

	EntityManager* entityManager;
};
#endif