#ifndef SPHEREGRAPHICS_H
#define SPHEREGRAPHICS_H

#include "../utilities/Logger.h"
#include "GraphicsComponent.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class SphereGraphicsComponent : public GraphicsComponent {
public:
	SphereGraphicsComponent();

	virtual void update(Entity& entity);
	void draw(Entity& entity);

private:

	unsigned int VAO, VBO;

	Shader* m_shader;
	std::vector<GLfloat> vertices;

};

#endif