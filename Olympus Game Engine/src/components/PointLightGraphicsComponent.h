#ifndef POINTLIGHTGRAPHICS_H
#define POINTLIGHTGRAPHICS_H

#include "../utilities/Logger.h"
#include "GraphicsComponent.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class PointLightGraphicsComponent : public GraphicsComponent {
public:
	PointLightGraphicsComponent();

	virtual void update(Entity& entity);
	
	glm::vec3 getLightColor();
	void setLightColor(glm::vec3);

private:
	int m_radius, m_stacks, m_slices;
	unsigned int VAO, VBO;

	glm::vec3 m_color;

	Shader* m_shader;
	std::vector<GLfloat> vertices;
};

#endif