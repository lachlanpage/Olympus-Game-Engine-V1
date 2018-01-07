#ifndef QUADGRAPHICS_H
#define QUADGRAPHICS_H

#include "../utilities/Logger.h"
#include "GraphicsComponent.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class QuadGraphicsComponent : public GraphicsComponent {
public:
	QuadGraphicsComponent();

	virtual void update(Entity& entity);
	void updateQuadShader(Shader*);
private:
	unsigned int VAO, VBO;
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