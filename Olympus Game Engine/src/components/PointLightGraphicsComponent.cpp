#include "PointLightGraphicsComponent.h"

PointLightGraphicsComponent::PointLightGraphicsComponent(){

	//fix this up with a radius method and stacks and slices approach 
	//something like this: https://www.opengl.org/discussion_boards/showthread.php/159584-sphere-generation
	m_shader = ResourceManager::Instance()->loadShader("src/shaders/pointLight.vs", "src/shaders/pointLight.fs");

}
void PointLightGraphicsComponent::update(Entity& entity) {

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), &entity.getPosition(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	m_shader->use();

	//lightradius, lightpoisiton, lightcolor 



	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	m_shader->setVec3("lightPosition", entity.getPosition());
	m_shader->setFloat("lightRadius", 10);
	m_shader->setVec3("lightColor", glm::vec3(1.0, 0.0, 0.0));
	glBindVertexArray(VAO);
	Renderer::Instance()->updateLightShader(m_shader);
	sendToRenderer(GL_TRIANGLES, 0, 10);
}
