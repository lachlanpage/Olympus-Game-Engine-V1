#ifndef BILLBOARDCOMPONENT_H
#define BILLBOARDCOMPONENT_H 
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../core/Shader.h"
#include "../core/ResourceManager.h"
class BillboardComponent : public Component {
public:
	BillboardComponent();
	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);

private:

	Shader* m_shader;

	unsigned int quadVAO;
	unsigned int quadVBO;

	unsigned int grassTexture;

	std::vector<glm::vec3> positionList;

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
};
#endif

