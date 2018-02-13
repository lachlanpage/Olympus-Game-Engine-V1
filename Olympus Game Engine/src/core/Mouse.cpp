#include "Mouse.h"

Mouse* Mouse::m_Instance = 0;

Mouse* Mouse::Instance() {
	if (m_Instance == 0)
		Logger::Instance()->write("INVALID MOUSE ACCESS");
	return m_Instance;
}

Mouse* Mouse::Instance(MessageBus* messageBus) {
	if (m_Instance == 0)
		m_Instance = new Mouse(messageBus);
	return m_Instance;

}

Mouse::Mouse( MessageBus* messageBus) : BusNode(messageBus) {
	projectionMatrix = Settings::Instance()->projection;
	blockClickID = -1;
}

void Mouse::checkIntersection(glm::vec3 position) {
}

void Mouse::onNotify(Message message) {
	if (message.getEvent() == "MOUSE_LEFT_CLICK") {
		std::cout << "THIS IS CALLED" << std::endl;
		blockClickID = m_blockIntersectionID;
	}
}

void Mouse::update(std::vector<Entity*> entityList) {
	m_entityList = entityList;

	viewMatrix = Camera::Instance()->getViewMatrix();
	currentRay = calculateMouseRay();

	glm::vec3 camPos = Camera::Instance()->getPosition();
	int RANGE = 13; 

	glm::vec3 start = camPos + glm::vec3(currentRay.x * RANGE, currentRay.y * RANGE, currentRay.z * RANGE);

	glm::vec3 startOfRay = getPointOnRay(currentRay, 0);
	glm::vec3 endOfRay = getPointOnRay(currentRay, 10);

	bool blockIntersection = false;
	float rayLength = 0;
	float step_size = 1;
	while (rayLength < RANGE && blockIntersection == false) {
		glm::vec3 testRayPoint = getPointOnRay(currentRay, rayLength);
		testRayPoint = glm::vec3(floor(testRayPoint.x), floor(testRayPoint.y), floor(testRayPoint.z));
		for (auto entity : entityList) {
			//is slected could cause problems if loops exists early before setting state for all entitys back to not selected
			//maybe update each entity to is selected is false each update?
			glm::vec3 entityPosition = entity->getPosition();
			entity->is_selected = false;
			if (testRayPoint.x == entityPosition.x && testRayPoint.y == entityPosition.y && testRayPoint.z == entityPosition.z) {
				m_currentPoint = glm::vec3(entityPosition);
				entity->is_selected = true;
				blockIntersection = true;
				m_blockIntersectionID = entity->m_ID;
				break;
			}
			else {
				m_blockIntersectionID = -1;
			}
		}
		rayLength = rayLength + step_size;
	}


	//update is selected
	for (auto entity : m_entityList) {
		if (entity->m_ID == blockClickID) {
			entity->is_selected = true;
			GUIManager::Instance()->setEntityEditor(entity);
		}
		else {
			//entity->is_selected = false;
		}
	}

	//render gui accordingly 
	if (blockClickID == -1) {
		GUIManager::Instance()->renderEntityEditor(false);
	}
	else {
		GUIManager::Instance()->renderEntityEditor(true);
	}
}

glm::vec3 Mouse::getCurrentPoint() {
	return m_currentPoint;
}

glm::vec3 Mouse::getPointOnRay(glm::vec3 ray, float distance) {
	//returns the beginning point and end points of ray governed by distanced
	glm::vec3 cameraPosition = Camera::Instance()->getPosition();
	glm::vec3 startofRay = cameraPosition;
	glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z*distance);
	glm::vec3 finalRay = startofRay + scaledRay;
	return finalRay;
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
	float xnorm = (2 * x) / Settings::Instance()->window_width - 1;
	float ynorm = (2 * y) / Settings::Instance()->window_height - 1;
	return glm::vec2(xnorm, ynorm);
}