#include "SceneManager.h"

SceneManager::SceneManager(const char* theScene, EntityManager *manager) {

	entityManager = manager;

	doc.LoadFile(theScene);

	if (doc.ErrorID() != 0) {
		std::cout << "ERROR LOADING SCENE:: " << doc.ErrorID() << std::endl;
	}
	else {
		parseScene();
	}

}

void SceneManager::parseScene() {

	tinyxml2::XMLNode* root = doc.FirstChildElement("Scene");
	if (root == nullptr)
		std::cout << "ROOT NODE ERROR" << std::endl;

	//Iterate over all entities
	for (tinyxml2::XMLNode* entity = root->FirstChildElement("Entity"); entity != NULL; entity = entity->NextSiblingElement("Entity")) {

		tinyxml2::XMLNode* positionNode = entity->FirstChildElement("Position");

		std::string positionX = positionNode->FirstChildElement("X")->GetText();
		std::string positionY = positionNode->FirstChildElement("Y")->GetText();
		std::string positionZ = positionNode->FirstChildElement("Z")->GetText();

		float posX = std::stof(positionX);
		float posY = std::stof(positionY);
		float posZ = std::stof(positionZ);

		tinyxml2::XMLNode* scaleNode = entity->FirstChildElement("Scale");
		
		std::string scaleX = scaleNode->FirstChildElement("X")->GetText();
		std::string scaleY = scaleNode->FirstChildElement("Y")->GetText();
		std::string scaleZ = scaleNode->FirstChildElement("Z")->GetText();

		float scX = std::stof(scaleX);
		float scY = std::stof(scaleY);
		float scZ = std::stof(scaleZ);

		tinyxml2::XMLNode* rotationNode = entity->FirstChildElement("Rotation");

		std::string rotationX = rotationNode->FirstChildElement("X")->GetText();
		std::string rotationY = rotationNode->FirstChildElement("Y")->GetText();
		std::string rotationZ = rotationNode->FirstChildElement("Z")->GetText();

		float rotX = std::stof(rotationX);
		float rotY = std::stof(rotationY);
		float rotZ = std::stof(rotationZ);


		Entity *newEntity = new Entity(glm::vec3(posX, posY, posZ));
		newEntity->setScale(glm::vec3(scX, scY, scZ));
		newEntity->setRotation(glm::vec3(rotX, rotY, rotZ));
		newEntity->addComponent(new ModelComponent("models/cube/cube.obj"));
		entityManager->addEntity(newEntity);

		//std::cout << scaleX.c_str() << std::endl;

	}
	
}