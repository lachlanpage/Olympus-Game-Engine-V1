#include "PlaneGraphicsComponent.h"

PlaneGraphicsComponent::PlaneGraphicsComponent() {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/cube.vs", "src/shaders/cube.fs");
	m_shadowShader = ResourceManager::Instance()->loadShader("src/shaders/shadow.vs", "src/shaders/shadow.fs");

	m_albedoTextureFilename = "textures/albedo_container.png";
	m_specularTextureFilename = "textures/specular_container.png";

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates) + sizeof(normalData), NULL, GL_STATIC_DRAW);
	//attributes for shader
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//offset for texture coordinates (location = 1), normalData location = 2
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textureCoordinates), textureCoordinates);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textureCoordinates), sizeof(normalData), normalData);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertices) + sizeof(textureCoordinates)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	albedo_texture = ResourceManager::Instance()->loadTexture(m_albedoTextureFilename);
	specular_texture = ResourceManager::Instance()->loadTexture(m_specularTextureFilename);
}

void PlaneGraphicsComponent::postInit(Entity& entity) {

}

unsigned int PlaneGraphicsComponent::getAlbedoTexture() {
	return albedo_texture;
}

unsigned int PlaneGraphicsComponent::getSpecularTexture() {
	return specular_texture;
}

std::string PlaneGraphicsComponent::getAlbedoTextureFilename() {
	return m_albedoTextureFilename;
}

std::string PlaneGraphicsComponent::getSpecularTextureFilename() {
	return m_specularTextureFilename;
}

void PlaneGraphicsComponent::setAlbedoTexture(std::string filename) {
	std::cout << "SET ALBEDO CALLED";
	albedo_texture = ResourceManager::Instance()->loadTexture(filename);
	m_albedoTextureFilename = filename;
}

void PlaneGraphicsComponent::setSpecularTexture(std::string filename) {
	std::cout << "SET SPEC CALLED" << std::endl;
	specular_texture = ResourceManager::Instance()->loadTexture(filename);
	m_specularTextureFilename = filename;
}

void PlaneGraphicsComponent::renderShadow(Entity& entity) {
	m_shadowShader->use();
	m_shadowShader->setMat4("view", Settings::Instance()->depthViewMatrix);
	m_shadowShader->setMat4("projection", Settings::Instance()->projectionMatrix);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	model = glm::scale(model, entity.getScale());
	//x rotation
	model = glm::rotate(model, glm::radians(entity.getRotation().x), glm::vec3(1, 0, 0));
	//y rotation 
	model = glm::rotate(model, glm::radians(entity.getRotation().y), glm::vec3(0, 1, 0));
	//z rotation
	model = glm::rotate(model, glm::radians(entity.getRotation().z), glm::vec3(0, 0, 1));
	m_shadowShader->setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void PlaneGraphicsComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	m_shader->setBool("isSelected", entity.is_selected);

	//texture stuff
	m_shader->setInt("albedo_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo_texture);

	m_shader->setInt("specular_texture", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_texture);

	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	model = glm::scale(model, entity.getScale());
	//rotation axis of model {x,y,z}
	model = glm::rotate(model, glm::radians(entity.getRotation().x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().z), glm::vec3(0, 0, 1));
	m_shader->setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
