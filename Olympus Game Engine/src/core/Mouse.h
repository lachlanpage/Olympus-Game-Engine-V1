#ifndef MOUSEPICKER_H 
#define MOUSEPICKER_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"
#include "../core/Entity.h"
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <iostream>
#include "GUIManager.h"
#include <bullet/btBulletDynamicsCommon.h>
class Mouse : public BusNode {
public:
	static Mouse* Instance();
	static Mouse* Instance(MessageBus* messageBus);
	void update(std::vector<Entity*>, btDiscreteDynamicsWorld* dynamicsWorld);
	void checkIntersection(glm::vec3 position);
	glm::vec3 getCurrentPoint();
	glm::vec3 m_currentPoint;

	int m_blockIntersectionID;
	int blockClickID;
	bool isLeftClick;
	bool leftClickedReturned;

	bool getLeftClickState();

protected:
private:
	Mouse(MessageBus* messageBus);
	void onNotify(Message message);

	std::vector<Entity*> m_entityList;

	glm::vec3 getPointOnRay(glm::vec3 ray, float distance);
	glm::vec3 calculateMouseRay();
	glm::vec2 Mouse::normDeviceCoords(int x, int y);
	glm::vec4 Mouse::toEyeCoords(glm::vec4 clipCoords);
	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 currentRay;
	static Mouse* m_Instance;

};


#endif