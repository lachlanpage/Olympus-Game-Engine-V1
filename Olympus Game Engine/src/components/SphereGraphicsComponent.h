#ifndef SPHEREGRAPHICS_H
#define SPHEREGRAPHICS_H

#include "../utilities/Logger.h"
#include "GraphicsComponent.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

class SphereGraphicsComponent : public Component {
public:
	SphereGraphicsComponent(float,float);

	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);

private:
	int m_stacks, m_slices;
	unsigned int VAO, VBO, EBO;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	unsigned int indexCount;

	//required pbr textures for IBL 
	unsigned int irradianceMap; 
	unsigned int prefilterMap;
	unsigned int brdfMap;


	float m_roughness;
	float m_metallic;

	Shader* m_shader;
	Shader* m_shadowShader;
	std::vector<GLfloat> vertices;

};

#endif