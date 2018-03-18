#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H 
#include <string>
#include "Model.h"
#include "Mesh.h"
#include "Component.h"
#include "../core/Entity.h"
#include "../core/ResourceManager.h"

#include <iostream>
class ModelComponent : public Component{
public:

	ModelComponent(char* filepath);
	ModelComponent(char* filepath, std::string colourFlag);
	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);

	Model* getModel();
private:
	Model *ourModel;
	Shader *m_shader;
	Shader *m_shadowShader;

	//required pbr textures for IBL 
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfMap;
};

#endif