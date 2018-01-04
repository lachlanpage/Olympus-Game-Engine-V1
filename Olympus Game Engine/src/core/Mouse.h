#ifndef MOUSEPICKER_H 
#define MOUSEPICKER_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <iostream>
class Mouse {
public:
	static Mouse* Instance();
	void update(glm::mat4 view);
	void checkIntersection(glm::vec3 position);

protected:
private:
	Mouse();
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