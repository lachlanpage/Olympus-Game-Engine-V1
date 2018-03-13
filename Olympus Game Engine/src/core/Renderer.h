#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include "Shader.h"
#include "../utilities/Camera.h"
#include <iostream>
#include "ResourceManager.h"
#include <random>
class Renderer {
public:
	static Renderer* m_Instance;
	static Renderer* Instance();
	void start();
	void stop();
	void SSAO();
	void Flush();
	void lightingPassStart();
	void lightingPassStop();

	void startShadowMap();
	void stopShadowMap();
	void render(GLenum, GLint, GLsizei);
	void updateQuadShader(Shader *);
	void updateLightShader(Shader *);
	void getBufferTextures(Shader *);
	
private:
	unsigned int m_fbo;

	void renderSkybox();

	//shaders used for post-processing effects
	Shader *shaderSSAO;
	Shader *shaderSSAOBlur;
	Shader *shaderFinalPass;
	Shader *crosshairShader;
	Shader *cubemapShader;
	Shader *backgroundShader;
	Shader *irradianceShader;
	Shader *prefilterShader;
	Shader *brdfShader;
	std::vector<glm::vec3> ssaoNoise;
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;

	Renderer();
	void renderQuad();
	void renderCube();
	unsigned int quadVAO = 0; 
	unsigned int quadVBO;

	unsigned int cubeVAO = 0; 
	unsigned int cubeVBO = 0;

	unsigned int hdrTexture;
	unsigned int cubemapEnvironment;

	//normal, diffuse, position, tex
	GLuint m_textures[4];
	GLuint m_depthTexture;
	GLuint renderedTexture;
	GLuint renderedTexture1;
	GLuint renderedTexture2;
	GLuint renderedTexture3;
	GLuint depthrenderbuffer;
	GLuint renderbuffer;
	GLuint framebuffer;
	GLuint lightingbuffer;
	GLuint shadowBuffer;
	GLuint ssaoBuffer, ssaoBlurBuffer;
	GLuint ssaoColorBuffer, ssaoColorBufferBlur;
	GLuint cubemapBuffer, cubemapRenderBuffer;

	unsigned int lightingTexture;
	unsigned int colorTexture;
	unsigned int normalTexture;
	unsigned int positionTexture;
	unsigned int specularTexture;
	unsigned int eyePositionTexture;
	unsigned int eyeNormalTexture;
	unsigned int noiseTexture;

	unsigned int brdfTexture;

	unsigned int irradMap;
	unsigned int prefilterMap;

	unsigned int shadowDepthTexture;

	unsigned int texture, texture2;

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};
};

#endif