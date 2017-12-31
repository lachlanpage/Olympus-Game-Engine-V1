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

	unsigned int VAO, VBO, m_vboIndex;
	int m_stacks, m_slices;
	float m_radius;
	glm::vec3 m_color;

	Shader* m_shader;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
};

#endif