#include "Renderer.h"

Renderer* Renderer::m_Instance = nullptr;
std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);

Renderer* Renderer::Instance() {
	if (m_Instance == nullptr)
		m_Instance = new Renderer();
	return m_Instance;
}

void Renderer::render(GLenum mode, GLint first, GLsizei count) {
	glDrawArrays(mode, first, count);
}

void Renderer::start() {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGenerateMipmap(GL_TEXTURE_2D);

	//render skybox first --- hacky !!!!
	renderSkybox();
}


void Renderer::renderSkybox() {
	//render skybox first --- hacky !!!!
	backgroundShader->use();
	backgroundShader->setMat4("view", Camera::Instance()->getViewMatrix());
	backgroundShader->setMat4("projection", Settings::Instance()->projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradMap);
	glDepthFunc(GL_LEQUAL);
	renderCube();
}

void Renderer::stop() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::getBufferTextures(Shader *shader) {
	shader->setInt("colorTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

	shader->setInt("normalTexture", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	shader->setInt("positionTexture", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, positionTexture);

	shader->setInt("shadowTexture", 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
}

void Renderer::updateLightShader(Shader* shader) {
	shader->setVec3("cameraPosition", Camera::Instance()->getPosition());
	shader->setInt("colorTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

	shader->setInt("normalTexture", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	shader->setInt("positionTexture", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, positionTexture);

	shader->setInt("shadowTexture", 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);

	shader->setInt("specularTexture", 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, specularTexture);

	shader->setInt("ssaoTexture", 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
}

void Renderer::lightingPassStart() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lightingbuffer);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer::lightingPassStop() {
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::startShadowMap() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowBuffer);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::stopShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::SSAO() {
	//generator ssao texture
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderSSAO->use();
	shaderSSAO->setMat4("viewMatrix", Camera::Instance()->getViewMatrix());
	//send kernel + rotation 
	for (unsigned int i = 0; i < 64; i++)
		shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	shaderSSAO->setMat4("projection", Settings::Instance()->projection);
	//change uniforms
	shaderSSAO->setFloat("radius", Settings::Instance()->ssaoRadius);
	shaderSSAO->setFloat("bias", Settings::Instance()->ssaoBias);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, eyePositionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, eyeNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//blur ssao to remove noise 
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	shaderSSAOBlur->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void Renderer::updateQuadShader(Shader* shader) {
	//this is to update textures for final quad shader
	shader->setFloat("textureSelector", Settings::Instance()->m_textureSelector);
	//need to delete
	//teseting for shadow stuff 
	shader->setMat4("biasMatrix", Settings::Instance()->biasMatrix);

	shader->setMat4("view", Camera::Instance()->getViewMatrix());

	shader->setInt("colorTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

	shader->setInt("normalTexture", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	shader->setInt("positionTexture", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, positionTexture);

	shader->setInt("lightTexture", 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightingTexture);

	shader->setInt("shadowTexture", 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);

	shader->setInt("specularTexture", 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, specularTexture);

	shader->setInt("ssaoTexture", 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	shader->setInt("ssaoTextureBlur", 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);

	shader->setVec3("cameraPosition", Camera::Instance()->getPosition());

	shader->setFloat("exposure", Settings::Instance()->exposure);
	shader->setFloat("gamma", Settings::Instance()->gamma);
}

Renderer::Renderer() {
	//create post processing shaders
	shaderSSAO = new Shader("src/shaders/ssao.vs", "src/shaders/ssao.fs");
	shaderSSAOBlur = new Shader("src/shaders/ssao.vs", "src/shaders/ssao_blur.fs");
	shaderFinalPass = ResourceManager::Instance()->loadShader("src/shaders/passthrough.vs", "src/shaders/simpleTexture.fs");
	crosshairShader = ResourceManager::Instance()->loadShader("src/shaders/passthrough.vs", "src/shaders/crosshair.fs");

	cubemapShader = ResourceManager::Instance()->loadShader("src/shaders/cubemap.vs", "src/shaders/cubemap.fs");
	backgroundShader = ResourceManager::Instance()->loadShader("src/shaders/background.vs", "src/shaders/background.fs");
	irradianceShader = ResourceManager::Instance()->loadShader("src/shaders/cubemap.vs", "src/shaders/irradiance_convolution.fs");


	//load HDR environment map 
	hdrTexture = ResourceManager::Instance()->loadTextureHDR("textures/ibl_hdr_radiance.png");

	//setup cubemap and render to fbo & texture
	glGenTextures(1, &cubemapEnvironment);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapEnvironment);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//projection and view matrices for 6 cubemap directions 
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//convert HDR equirectangular to cubemap
	cubemapShader->use();
	cubemapShader->setInt("equirectangularMap", 0);
	cubemapShader->setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glViewport(0, 0, 512, 512);
	glGenFramebuffers(1, &cubemapBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, cubemapBuffer);
	for (unsigned int i = 0; i < 6; i++) {
		cubemapShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapEnvironment, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);

	////////////////////////////////////////////


	glGenTextures(1, &irradMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradMap);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//use convolution shader to generate the environment map {solving integral by convolution}
	irradianceShader->use();
	irradianceShader->setInt("environmentMap", 0);
	irradianceShader->setMat4("projection", Settings::Instance()->projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapEnvironment);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, cubemapBuffer);

	for (unsigned int i = 0; i < 6; i++) {
		irradianceShader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Settings::Instance()->window_width, Settings::Instance()->window_height);


	//store texture in resourcemanger 
	ResourceManager::Instance()->storeTexture("irradianceMap", irradMap);


	////////////////////////// main framebuffer code ///////////////////

	//Initializes the G Buffer for MRT
	//Create FBO
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// color texture
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// normal txture
	glGenTextures(1, &normalTexture);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &positionTexture);
	glBindTexture(GL_TEXTURE_2D, positionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, positionTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//specular texture
	glGenTextures(1, &specularTexture);
	glBindTexture(GL_TEXTURE_2D, specularTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, specularTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// normal txture
	glGenTextures(1, &eyeNormalTexture);
	glBindTexture(GL_TEXTURE_2D, eyeNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, eyeNormalTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &eyePositionTexture);
	glBindTexture(GL_TEXTURE_2D, eyePositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D,eyePositionTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Depth buffer for framebuffer
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Settings::Instance()->window_width, Settings::Instance()->window_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER, renderbuffer);


	//attach texture and depth buffer to framebuffer
	GLuint attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	glDrawBuffers(7, attachments);
	glEnable(GL_DEPTH_TEST);

	//check for errors 
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAME BUFFER DID NOT WORK " << std::endl;

	//Framebuffer for the second render pass {lighting}
	//generate lighting framebuffer 
	glGenFramebuffers(1, &lightingbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,lightingbuffer);

	glGenTextures(1, &lightingTexture);
	glBindTexture(GL_TEXTURE_2D, lightingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightingTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint attachments2[1] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments2);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAME BUFFER DID NOT WORK " << std::endl;

	//Framebuffer for shadow mapping 
	glGenFramebuffers(1, &shadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	//use depth texture to make life simpler 
	glGenTextures(1, &shadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthTexture, 0);
	glDrawBuffer(GL_NONE); //dont need no colour buffer boiiiiii :) 

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAME BUFFER DID NOT WORK " << std::endl;

	//Framebuffers for SSAO processing 
	glGenFramebuffers(1, &ssaoBuffer);
	glGenFramebuffers(1, &ssaoBlurBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBuffer);

	//ssao color buffer time 
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO COL BUF NOT WORKING" << std::endl;
	
	//blur stage 
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurBuffer);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Settings::Instance()->window_width, Settings::Instance()->window_height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO COL BUF BLUR NOT WORKING" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//generate sample kernerl 

	//gen textures
	
	float resolution = 128.0;
	for (int i = 0; i < resolution; i++) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / resolution;
		scale = 0.1f + (scale*scale) * (1.0f - 0.1f);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	//generate noise texture 
	for (unsigned int i = 0; i < 16; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); //rotate around z axis tang space 
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void Renderer::Flush() {
	//render skybox and then final quad shader
	

	shaderFinalPass->use();
	updateQuadShader(shaderFinalPass);
	renderQuad();


	

	

}

void Renderer::renderQuad() {
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

}

void Renderer::renderCube() {
		// initialize (if necessary)
		if (cubeVAO == 0){
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}