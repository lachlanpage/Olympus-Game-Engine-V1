#include "ModelComponent.h"

ModelComponent::ModelComponent(char* filepath) {
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/model_loading.vs", "src/shaders/model_loading.fs");
	m_shadowShader = ResourceManager::Instance()->loadShader("src/shaders/shadow.vs", "src/shaders/shadow.fs");
	ourModel = new Model(filepath);
}

void ModelComponent::update(Entity& entity) {
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);

	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	model = glm::scale(model, entity.getScale());
	//rotation axis of model {x,y,z}
	model = glm::rotate(model, glm::radians(entity.getRotation().x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().z), glm::vec3(0, 0, 1));
	m_shader->setMat4("model", model);
	ourModel->Draw(m_shader);
}
void ModelComponent::renderShadow(Entity& entity){
	m_shadowShader->use();
	m_shadowShader->setMat4("view", Settings::Instance()->depthViewMatrix);
	m_shadowShader->setMat4("projection", Settings::Instance()->projectionMatrix);

	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	model = glm::scale(model, entity.getScale());
	//rotation axis of model {x,y,z}
	model = glm::rotate(model, glm::radians(entity.getRotation().x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(entity.getRotation().z), glm::vec3(0, 0, 1));
	m_shadowShader->setMat4("model", model);
	ourModel->Draw(m_shader);
}
void ModelComponent::postInit(Entity& entity){}
Model* ModelComponent::getModel() { return ourModel; }