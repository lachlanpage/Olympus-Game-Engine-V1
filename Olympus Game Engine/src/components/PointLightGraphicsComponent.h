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
	void draw(Entity& entity);

private:
	int m_radius;
	unsigned int VAO, VBO;

	Shader* m_shader;
	std::vector<GLfloat> vertices;



};

#endif