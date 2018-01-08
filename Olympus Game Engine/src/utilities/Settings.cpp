#include "Settings.h"

Settings* Settings::m_Instance = 0;

Settings* Settings::Instance() {
	if (m_Instance == 0)
		m_Instance = new Settings();
	return m_Instance;
}

Settings::Settings() {
	//set all global settings here
	window_height = 1080;
	window_width = 1920;
	projection = glm::perspective(glm::radians(80.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
	

	//shadow map stuff 
	lightDir = glm::vec3(0.7, 0.3, 0.1);
	projectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -20, 20);
	depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	depthModelMatrix = glm::mat4(1.0);
	lightSpaceMatrix = projectionMatrix* depthViewMatrix * depthModelMatrix;
	depthMVP = projectionMatrix * depthViewMatrix * depthModelMatrix;
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
}

void Settings::setLightDirection(glm::vec3 direction) {
	lightDir = direction;
	depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	lightSpaceMatrix = projectionMatrix * depthViewMatrix * depthModelMatrix;
}
