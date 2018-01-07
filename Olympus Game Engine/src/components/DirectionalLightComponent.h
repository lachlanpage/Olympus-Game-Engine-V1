#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "../utilities/Logger.h"
#include "Component.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class DirectionalLightComponent : public Component {
public:
	DirectionalLightComponent(glm::vec3 direction);

	virtual void update(Entity& entity);
	void getBufferTextures();
	virtual void postInit(Entity& entity);
private:
	unsigned int VAO, VBO;
	glm::vec3 m_direction;
	Shader* m_shader;

	float vertices[18] = {

		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f
	};
	float textureCoordinates[12] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};
};

#endif