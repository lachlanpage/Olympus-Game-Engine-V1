#ifndef PLANEGRAPHICS_H
#define PLANEGRAPHICS_H

#include "../utilities/Logger.h"
#include "Component.h"
#include "../core/ResourceManager.h"
#include "../core/Shader.h"
#include "../utilities/Settings.h"
#include "../utilities/Camera.h"
#include "../utilities/stb_image.h"
class PlaneGraphicsComponent : public Component {
public:
	PlaneGraphicsComponent();

	virtual void update(Entity& entity);
	void renderShadow(Entity& entity);
	virtual void postInit(Entity& entity);

	unsigned int getAlbedoTexture();
	unsigned int getSpecularTexture();

	void setAlbedoTexture(std::string);
	void setSpecularTexture(std::string);

	std::string getAlbedoTextureFilename();
	std::string getSpecularTextureFilename();

private:
	unsigned int albedo_texture, specular_texture;
	unsigned int cubeVAO, cubeVBO;

	std::string m_albedoTextureFilename, m_specularTextureFilename;

	Shader* m_shader;
	Shader* m_shadowShader;

	float vertices[18] = {

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f

	};
	float textureCoordinates[12] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	float normalData[18] = {
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f
	};
};

#endif