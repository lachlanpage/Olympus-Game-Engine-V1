#ifndef CAMERA_H 
#define CAMERA_H 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BusNode.h"
#include "Logger.h"
#include "Time.h"
class Camera : public BusNode {
public:
	static Camera* m_Instance;
	static Camera* Instance();
	static Camera* Instance(glm::vec3 position, glm::vec3 front, glm::vec3 up, MessageBus* messageBus);

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	float deltaTime;

	glm::mat4 Camera::getViewMatrix();
	glm::vec3 getPosition();
	void onNotify(Message message);
	void handleInput(std::string movement);

	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch);

	void update_time(float delta);

private:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, MessageBus* messageBus);

	void update();

};

#endif