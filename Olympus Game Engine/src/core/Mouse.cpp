#include "Mouse.h"

Mouse* Mouse::m_Instance = 0;

Mouse* Mouse::Instance() {
	if (m_Instance == 0)
		m_Instance = new Mouse;
	return m_Instance;
}

Mouse::Mouse() {
	projectionMatrix = Settings::Instance()->projection;
}

void Mouse::checkIntersection(glm::vec3 position) {


}

void Mouse::update(glm::mat4 view) {
	viewMatrix = view;
	currentRay = calculateMouseRay();

	glm::vec3 camPos = Camera::Instance()->getPosition();
	int RANGE = 600; 

	glm::vec3 start = camPos + glm::vec3(currentRay.x * RANGE, currentRay.y * RANGE, currentRay.z * RANGE);


	std::cout << currentRay.x << " " << currentRay.y << " " <<  currentRay.z << " " << std::endl;
}

glm::vec3 Mouse::calculateMouseRay() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	glm::vec2 normalCoords = normDeviceCoords(x, y);
	//convert to clip space 
	glm::vec4 clipCoords = glm::vec4(normalCoords.x, normalCoords.y, -1, 1);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldCoords = toWorldCoords(eyeCoords);

	return worldCoords;
}

glm::vec3 Mouse::toWorldCoords(glm::vec4 eyeCoords) {
	glm::mat4 invView = glm::inverse(viewMatrix);
	glm::vec3 rayWorld = glm::vec4(invView * eyeCoords);
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	return glm::normalize(mouseRay);
}

glm::vec4 Mouse::toEyeCoords(glm::vec4 clipCoords) {
	glm::mat4 invProjection = glm::inverse(projectionMatrix);
	glm::vec4 eyeCoords = glm::vec4(invProjection*clipCoords);
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1, 0);
}

glm::vec2 Mouse::normDeviceCoords(int x, int y) {
	float xnorm = (2 * x) / 800 - 1;
	float ynorm = (2 * y) / 600 - 1;
	return glm::vec2(xnorm, ynorm);
}