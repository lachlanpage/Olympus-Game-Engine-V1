#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "../utilities/Logger.h"
#include "Component.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"

#include "../tiny_obj_loader.h"

class ModelComponent : public Component {
public:
	ModelComponent();

	virtual void update(Entity& entity);
	virtual void postInit(Entity& entity);
private:
	//struct Mesh;
	//struct Material;

	unsigned int VAO, VBO, m_vboIndex;
	

	std::vector<float> m_vertices;
	std::vector<float> normals;
	std::vector<float> textureCoordinates;

	Shader* m_shader;
	//std::vector<Mesh*> meshes; //meshes of the .obj model
	//std::vector<Material> materials; //materials of .obj model
};

#endif