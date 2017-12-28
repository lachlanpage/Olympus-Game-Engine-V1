#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "../utilities/Logger.h"
#include "Component.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class LightComponent : public Component {
public:
	LightComponent(float radius, glm::vec3 color);

	virtual void update(Entity& entity);
	virtual void postInit(Entity& entity);
private:

	void updateLightShader();

	unsigned int VAO, VBO;
	float m_radius;
	glm::vec3 m_color;

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