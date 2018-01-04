#include "Settings.h"

Settings* Settings::m_Instance = 0;

Settings* Settings::Instance() {
	if (m_Instance == 0)
		m_Instance = new Settings();
	return m_Instance;
}

Settings::Settings() {
	//set all global settings here
	projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 10000.0f);
	window_height = 600;
	window_width = 800;

	//shadow map stuff 
	lightDir = glm::vec3(-0.7, 0.3, 0.1);
	projectionMatrix = glm::ortho<float>(-30, 30, -30, 30, -30, 100);
	depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	depthModelMatrix = glm::mat4(1.0);
	depthMVP = projectionMatrix * depthViewMatrix * depthModelMatrix;
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
}
